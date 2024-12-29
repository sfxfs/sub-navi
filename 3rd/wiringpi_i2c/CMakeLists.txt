project(wiringpi_i2c)

add_library(${PROJECT_NAME}
            wiringPiI2C.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
