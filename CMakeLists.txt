cmake_minimum_required(VERSION 3.5)
project(sub-navi)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O3 -w -DNDEBUG")

add_subdirectory(3rd)
add_subdirectory(csv-json-config-sys)
add_subdirectory(json-rpc-server)
add_subdirectory(protobuf-commu)
add_subdirectory(main-ctrl)
