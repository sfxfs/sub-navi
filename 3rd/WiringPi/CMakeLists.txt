project(wiringpi)

add_library(${PROJECT_NAME}
            wiringPiI2C.c
            wiringSerial.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
