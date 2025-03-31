# Set the project name
project (luckfox_gpio)

# Add a library with the above sources
add_library(${PROJECT_NAME} luckfox_gpio.c luckfox_sysfs.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
