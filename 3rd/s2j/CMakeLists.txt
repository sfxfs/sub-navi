project(s2j)

add_library(${PROJECT_NAME} s2j.c)
target_link_libraries(${PROJECT_NAME} cjson)
