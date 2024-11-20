project(nanopb)

add_library(${PROJECT_NAME}
            pb_common.c
            pb_decode.c
            pb_encode.c)
