project(csv_parser)

add_library(${PROJECT_NAME} csv_parser.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
