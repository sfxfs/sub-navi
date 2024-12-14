/*
    MIT License

    Copyright (c) 2024 Xiao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "mjsonrpc.h"

#include <stdlib.h>
#include <string.h>


cJSON *mjrpc_response_ok(cJSON *result, cJSON *id)
{
    if (id == NULL || result == NULL)
        return NULL;

    cJSON *result_root = cJSON_CreateObject();
    if (result_root == NULL)
        return NULL;

    cJSON_AddStringToObject(result_root, "jsonrpc", "2.0");
    cJSON_AddItemToObject(result_root, "result", result);
    cJSON_AddItemToObject(result_root, "id", id);

    return result_root;
}

cJSON *mjrpc_response_error(int code, char *message, cJSON *id)
{
    if (id == NULL)
        return NULL;

    cJSON *result_root = cJSON_CreateObject();
    if (result_root == NULL)
        return NULL;

    cJSON *error_root = cJSON_CreateObject();
    if (error_root == NULL)
    {
        cJSON_Delete(result_root);
        return NULL;
    }

    cJSON_AddIntToObject(error_root, "code", code);
    if (message)
    {
        cJSON_AddStringToObject(error_root, "message", message);
        free(message);
    }

    cJSON_AddStringToObject(result_root, "jsonrpc", "2.0");
    cJSON_AddItemToObject(result_root, "error", error_root);
    cJSON_AddItemToObject(result_root, "id", id);

    return result_root;
}

static cJSON *invoke_callback(mjrpc_handle_t *handle,
                              char *method_name, cJSON *params, cJSON *id)
{
    cJSON *returned = NULL;
    int procedure_found = 0;
    mjrpc_ctx_t ctx;
    ctx.error_code = 0;
    ctx.error_message = NULL;
    int i = handle->cb_count;
    while (i--)
        if (!strcmp(handle->cb_array[i].name, method_name))
        {
            procedure_found = 1;
            ctx.data = handle->cb_array[i].arg;
            returned = handle->cb_array[i].function(&ctx, params, id);
            break;
        }

    if (!procedure_found)
        return mjrpc_response_error(JSON_RPC_CODE_METHOD_NOT_FOUND,
                                    strdup("Method not found."), id);
    else if (ctx.error_code)
        // Error in callback, custom error code and message
        return mjrpc_response_error(ctx.error_code, ctx.error_message, id);
    else
        // No error in callback, return the result
        return mjrpc_response_ok(returned, id);
}

static cJSON *rpc_handle_obj_req(mjrpc_handle_t *handle, cJSON *request)
{
    cJSON *version, *method, *params, *id;

    id = cJSON_GetObjectItem(request, "id");
    if (id == NULL)
        // No id, this is a notification
        return NULL;
    if (id->type == cJSON_NULL || id->type == cJSON_String || id->type == cJSON_Int)
    {
        cJSON *id_copy = NULL;
        if (id->type == cJSON_NULL)
            id_copy = cJSON_CreateNull();
        else
            id_copy = (id->type == cJSON_String) ?
                    cJSON_CreateString(id->valuestring) : cJSON_CreateNumber(id->valueint);

        version = cJSON_GetObjectItem(request, "jsonrpc");
        if (version == NULL || version->type != cJSON_String || strcmp("2.0", version->valuestring) != 0)
            return mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                        strdup("Valid request received: JSONRPC version error."),
                                        id_copy);

        method = cJSON_GetObjectItem(request, "method");
        if (method != NULL && method->type == cJSON_String)
        {
            params = cJSON_GetObjectItem(request, "params");

            return invoke_callback(handle, method->valuestring, params, id_copy);
        }
        return mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                    strdup("Valid request received: No 'method' member."), id_copy);
    }
    else
        // Invalid id type
        return mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                    strdup("Valid request received: 'id' member type error."),
                                    cJSON_CreateNull());;
}

static cJSON *rpc_handle_ary_req(mjrpc_handle_t *handle, cJSON *request)
{
    int array_size = cJSON_GetArraySize(request);
    if (array_size <= 0)
        return mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                    strdup("Valid request received: Empty JSON array."),
                                    cJSON_CreateNull());

    int valid_reqs = 0;
    cJSON *return_json_array = cJSON_CreateArray();
    for (int i = 0; i < array_size; i++)
    {
        cJSON *obj_req = rpc_handle_obj_req(handle, cJSON_GetArrayItem(request, i));
        if (obj_req)
        {
            cJSON_AddItemToArray(return_json_array, obj_req);
            valid_reqs ++;
        }
    }

    if (valid_reqs != 0)
        return return_json_array;
    // all requests are notifications or invalid
    return NULL;
}

// ----------------------------------------------------------------------------
// main functions

int mjrpc_add_method(mjrpc_handle_t *handle,
                     mjrpc_func function_pointer,
                     char *method_name, void *arg2func)
{
    if (handle == NULL)
        return MJRPC_RET_ERROR_HANDLE_NOT_INITIALIZED;
    if (function_pointer == NULL || method_name == NULL)
        return MJRPC_RET_ERROR_INVALID_PARAM;

    int i = handle->cb_count++;
    if (!handle->cb_array)
    {
        handle->cb_array = malloc(sizeof(struct mjrpc_cb));
        if (!handle->cb_array)
            return MJRPC_RET_ERROR_MEM_ALLOC_FAILED;
    }
    else
    {
        struct mjrpc_cb *ptr = realloc(handle->cb_array, sizeof(struct mjrpc_cb) *handle->cb_count);
        if (!ptr)
            return MJRPC_RET_ERROR_MEM_ALLOC_FAILED;
        handle->cb_array = ptr;
    }
    if ((handle->cb_array[i].name = strdup(method_name)) == NULL)
        return MJRPC_RET_ERROR_MEM_ALLOC_FAILED;
    handle->cb_array[i].function = function_pointer;
    handle->cb_array[i].arg = arg2func;

    return MJRPC_RET_OK;
}

static void cb_info_destroy(struct mjrpc_cb *info)
{
    if (info->name)
    {
        free(info->name);
        info->name = NULL;
    }
    if (info->arg)
    {
        free(info->arg);
        info->arg = NULL;
    }
}

int mjrpc_del_method(mjrpc_handle_t *handle, char *name)
{
    if (handle == NULL)
        return MJRPC_RET_ERROR_HANDLE_NOT_INITIALIZED;

    if (name == NULL)
    {
        for (int i = 0; i < handle->cb_count; i++)
            cb_info_destroy(&(handle->cb_array[i]));
        handle->cb_count = 0;
        free(handle->cb_array);
        handle->cb_array = NULL;
    }

    int found = 0;
    if (handle->cb_array)
    {
        for (int i = 0; i < handle->cb_count; i++)
        {
            if (found)
            {
                handle->cb_array[i - 1] = handle->cb_array[i];
            }
            else if (!strcmp(name, handle->cb_array[i].name))
            {
                found = 1;
                cb_info_destroy(&(handle->cb_array[i]));
            }
        }
        if (found)
        {
            handle->cb_count--;
            if (handle->cb_count)
            {
                struct mjrpc_cb *ptr = realloc(handle->cb_array, sizeof(struct mjrpc_cb) *handle->cb_count);
                if (!ptr)
                    return MJRPC_RET_ERROR_MEM_ALLOC_FAILED;

                handle->cb_array = ptr;
            }
            else
            {
                free(handle->cb_array);
                handle->cb_array = NULL;
            }
        }
    }
    else
        return MJRPC_RET_ERROR_NOT_FOUND;

    return MJRPC_RET_OK;
}

char *mjrpc_process_str(mjrpc_handle_t *handle,
                        const char *reqeust_str,
                        int *ret_code)
{
    cJSON *request = cJSON_Parse(reqeust_str);
    cJSON *response = mjrpc_process_cjson(handle, request, ret_code);
    cJSON_Delete(request);
    if (response)
    {
        char *response_str = cJSON_PrintUnformatted(response);
        cJSON_Delete(response);
        return response_str;
    }
    return NULL;
}

cJSON *mjrpc_process_cjson(mjrpc_handle_t *handle,
                           cJSON *request_cjson,
                           int *ret_code)
{
    int ret = MJRPC_RET_OK;
    if (handle == NULL)
    {
        ret = MJRPC_RET_ERROR_HANDLE_NOT_INITIALIZED;
        if (ret_code)
            *ret_code = ret;
        return NULL;
    }

    if (request_cjson == NULL)
    {
        // Empty object or array or empty request
        ret = MJRPC_RET_ERROR_EMPTY_REQUEST;
        if (ret_code)
            *ret_code = ret;
        return mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                    strdup("Valid request received: Empty request."), cJSON_CreateNull());
    }

    cJSON *cjson_return = NULL;
    if (request_cjson->type == cJSON_Array)
    {
        cjson_return = rpc_handle_ary_req(handle, request_cjson);
        ret = MJRPC_RET_OK;
    }
    else if (request_cjson->type == cJSON_Object)
    {
        cjson_return = rpc_handle_obj_req(handle, request_cjson);
        ret = MJRPC_RET_OK;
    }
    else
    {
        cjson_return = mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                            strdup("Valid request received: Not a JSON object or array."),
                                            cJSON_CreateNull());
        ret = MJRPC_RET_ERROR_PARSE_FAILED;
    }
    if (ret_code)
        *ret_code = ret;
    return cjson_return;
}
