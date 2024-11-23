add_executable(subnavi main.c)

target_link_libraries(subnavi
    m
    pthread
    cjson
    jsonrpc-c
    clog
    s2j
    nanopb
    ${ev}
)
