aux_source_directory(csv-json-config-sys cfg_sys_srcs)
aux_source_directory(json-rpc-server jsonrpc_srcs)
aux_source_directory(protobuf-commu protobuf_srcs)

add_executable(subnavi main.c
                        ${cfg_sys_srcs}
                        ${jsonrpc_srcs}
                        ${protobuf_srcs})

target_link_libraries(subnavi
    m
    pthread
    cjson
    jsonrpc-c
    clog
    s2j
    csv_parser
    nanopb
    ${ev}
)
