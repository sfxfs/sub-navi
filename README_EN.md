# SUB-NAVI

## Introduction

This project (SUB-NAVI) is a general-purpose underwater robot control program based on the Linux platform. It is designed to work in conjunction with [ESP-SUB-MASTER](https://github.com/sfxfs/esp-sub-master).

SUB-NAVI can achieve the following functionalities:

- Communication with a PC-based host using HTTP protocol and JSON-RPC 2.0 (using IO listening for improved performance)
- Serial communication with SUB-MASTER based on Protocol Buffers (for controlling underlying peripherals, thrusters, robotic arms, etc.)
- External connection of sensors and other peripherals via serial port
- Configuration management based on the file system (csv, json)
- Motion control algorithms

## Compilation

This project is built on `Rockchip rv1103` with an `armhf` architecture. Please use the `arm-rockchip830-linux-uclibcgnueabihf` compiler.

Compile using Linux and ensure the following are installed:

- Cross-compiler (added to PATH)
- `cmake`

Navigate to the project root directory and enter the following command to compile:

```shell
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-g++ && cmake --build build --config Release
```

If you are using VSCode for compilation, you also need to install:

- The corresponding CMake project compilation plugin
- `ninja-build`

Open the project files in VSCode, select the appropriate toolchain in the pop-up dialog, and choose "Build" in the bottom bar to compile.

## Development

The project mainly consists of three root directory folders:

- `3rd`: Contains third-party libraries and common code
- `config-header`: Contains global configuration header files
- `user`: Contains user project source code

The `user` folder primarily includes several functional modules:

- `control`: Robot control algorithms
- `csv-json-config-sys`: Program configuration system code
- `json-rpc-server`: JSON-RPC server application code
- `peripheral`: Drivers for related peripherals
- `protobuf-commu`: Communication code with SUB-MASTER