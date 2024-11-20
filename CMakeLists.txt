cmake_minimum_required(VERSION 3.5)
project(sub-navi)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O3 -w -DNDEBUG")

include_directories(3rd/cjson/include
                    3rd/clog/include
                    3rd/s2j/include
                    3rd/nanopb/include
                    3rd/libev/include
                    3rd/csv_parser/include)

add_subdirectory(3rd)
add_subdirectory(csv-json-config-sys)
add_subdirectory(json-rpc-server)
add_subdirectory(protobuf-commu)
add_subdirectory(main-ctrl)
