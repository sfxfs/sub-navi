aux_source_directory(csv-json-config-sys cfg_sys_srcs)
aux_source_directory(json-rpc-server jsonrpc_srcs)
aux_source_directory(protobuf-commu protobuf_srcs)
aux_source_directory(peripheral peripheral_srcs)
aux_source_directory(control control_srcs)

add_executable(${PROJECT_NAME} 
    main.c
    ${cfg_sys_srcs}
    ${jsonrpc_srcs}
    ${protobuf_srcs}
    ${peripheral_srcs}
    ${control_srcs})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/config_header)

target_link_libraries(${PROJECT_NAME}
    m
    pthread
    cjson
    jsonrpc-c
    clog
    s2j
    csv_parser
    nanopb
    linux_uart
    ${ev})

file(COPY ${PROJECT_SOURCE_DIR}/user/csv-json-config-sys/config-files/frame_factor.csv DESTINATION ${PROJECT_SOURCE_DIR}/output)
