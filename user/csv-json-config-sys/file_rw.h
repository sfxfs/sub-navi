#ifndef SUB_NAVI_CFG_JSON_FILE_READ_H
#define SUB_NAVI_CFG_JSON_FILE_READ_H

#include "navi-type.h"

/// @brief read config file
/// @param file_path path to config file
/// @return content of config file (NULL if error, and need to free() after use)
void *navi_read_from_file(const char *file_path);

/// @brief write content to config file
/// @param file_path path to config file
/// @param content content to write
/// @return 0 for success, others for fail
navi_ret_t navi_write_to_file(const char *file_path, const void *content);

#endif