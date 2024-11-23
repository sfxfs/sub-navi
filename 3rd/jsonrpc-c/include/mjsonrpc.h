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

#ifndef _MJSONRPC_H_
#define _MJSONRPC_H_

#include "cJSON.h"

#define JSON_RPC_2_0_PARSE_ERROR        -32700
#define JSON_RPC_2_0_INVALID_REQUEST    -32600
#define JSON_RPC_2_0_METHOD_NOT_FOUND   -32601
#define JSON_RPC_2_0_INVALID_PARAMS     -32603
#define JSON_RPC_2_0_INTERNAL_ERROR     -32693
// -32000 to -32099 Reserved for implementation-defined server-errors.

/**
 * @brief mjrpc error code
 */
enum mjrpc_error_return
{
    MJRPC_RET_OK,
    MJRPC_RET_ERROR_MEM_ALLOC_FAILED,
    MJRPC_RET_ERROR_NOT_FOUND,
    MJRPC_RET_ERROR_EMPTY_REQUEST,
    MJRPC_RET_ERROR_PARSE_FAILED,
};

/**
 * @brief jsonrpc callback function context
 */
typedef struct
{
    void *data;
    int error_code;
    char *error_message;
} mjrpc_ctx_t;

typedef cJSON *(*mjrpc_func)(mjrpc_ctx_t *context,
                                 cJSON *params,
                                 cJSON *id);

/**
 * @brief jsonrpc callback function
 */
struct mjrpc_cb
{
    char *name;
    mjrpc_func function;
    void *arg;
};

/**
 * @brief mjrpc handler
 */
typedef struct mjrpc_handler
{
    int cb_count;
    struct mjrpc_cb *cb_array;
} mjrpc_handler_t;

/**
 * @brief build a jsonrpc response with result
 * @param result call result
 * @param id client request id
 * @return cjson pointer
 */
cJSON *mjrpc_response_ok(cJSON *result, cJSON *id);

/**
 * @brief build a jsonrpc response with error
 * @param code error code
 * @param message error message
 * @param id client request id
 * @return cjson pointer
 */
cJSON *mjrpc_response_error(int code, char *message, cJSON *id);

/**
 * @brief add a method to jsonrpc handler
 * @param handler mjrpc handler
 * @param function_pointer callback function
 * @param method_name method name
 * @param arg2func argument to callback function
 * @return mjrpc_error_return
 */
int mjrpc_add_method(mjrpc_handler_t *handler,
                        mjrpc_func function_pointer,
                        char *method_name, void *arg2func);

/**
 * @brief delete a method from jsonrpc handler
 * @param handler mjrpc handler
 * @param method_name method name
 * @return mjrpc_error_return
 */
int mjrpc_del_method(mjrpc_handler_t *handler, char *method_name);

/**
 * @brief process a string typed jsonrpc request
 * @param handler mjrpc handler
 * @param reqeust_str request string
 * @param ret_code return code
 * @return response string (need to be free)
 */
char *mjrpc_process_str(mjrpc_handler_t *handler,
                            const char *reqeust_str,
                            int *ret_code);

/**
 * @brief process a cjson typed jsonrpc request
 * @param handler mjrpc handler
 * @param request_cjson request cjson
 * @param ret_code return code
 * @return response cjson pointer (need to be free)
 */
cJSON *mjrpc_process_cjson(mjrpc_handler_t *handler,
                            cJSON *request_cjson,
                            int *ret_code);

#endif
