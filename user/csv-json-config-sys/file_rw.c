#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"

#include "navi-type.h"

#include "file_rw.h"

void *navi_read_from_file(const char *file_path)
{
    if (file_path == NULL)
        return NULL;
    FILE *file;
    long file_size;
    char *buffer;

    // 打开文件
    file = fopen(file_path, "r");
    if (file == NULL)
    {
        log_error("Unable to open config file.");
        return NULL;
    }

    // 定位文件末尾以获取文件大小
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // 分配内存来存储文件内容
    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL)
    {
        log_error("Failed to alloc memory.");
        fclose(file);
        return NULL;
    }

    // 读取文件内容到缓冲区
    int bytes_read = fread(buffer, file_size, 1, file);
    if (bytes_read != 1)
    {
        log_error("Failed read from file.");
        fclose(file);
        free(buffer);
        return NULL;
    }
    log_info("Successfully read %ld bytes.", file_size);

    // 在缓冲区末尾添加字符串结束符
    buffer[file_size] = '\0';

    // 关闭文件
    fclose(file);

    return buffer;
}

navi_ret_t navi_write_to_file(const char *file_path, const void *content)
{
    if (content == NULL || file_path == NULL)
        return NAVI_RET_ARG_ERROR;
    FILE *file;

    // 打开文件以进行写入
    file = fopen(file_path, "w");
    if (file == NULL)
    {
        file = fopen(file_path, "wt+");
        if (file == NULL)
        {
            log_error("Unable to open config file.");
            return NAVI_RET_FAIL;
        }
    }

    // 写入内容到文件
    if (fprintf(file, "%s", (const char *)content) < 0)
    {
        log_error("Write to file failed.");
    }

    // 关闭文件
    fclose(file);
    return NAVI_RET_SUCCESS;
}
