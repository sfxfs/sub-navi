#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "csv_parser.h"

#include "csv-frame-factor.h"


static int field_cb(csv_parser_t *parser, const char *data, size_t length, int row, int col)
{
    frame_factor_t *frame_factor = parser->data;

    // ignore header
    if (row >= 1 && col >= 1)
        frame_factor->value[row - 1][col - 1] = atof(data);

    return 0;
}

frame_factor_t *frame_factor_get_from_file(const char *config_file_path)
{
    int fd = open(config_file_path, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }

    csv_parser_t parser;
    csv_parser_settings_t settings;
    settings.delimiter = ',';
    settings.field_cb = field_cb;

    csv_parser_init(&parser);
    parser.data = calloc(1, sizeof(frame_factor_t));

    char buffer[64];
    ssize_t nread;
    while ((nread = read(fd, buffer, sizeof(buffer))) > 0) {
        csv_parser_execute(&parser, &settings, buffer, nread);
    }

    close(fd);
    return parser.data;
}
