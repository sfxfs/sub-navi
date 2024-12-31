project(struct2json)

add_library(${PROJECT_NAME} s2j.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} cjson)
