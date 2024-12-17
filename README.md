# SUB-NAVI

***中文 | [English](README_EN.md)***

## 介绍

本项目（SUB-NAVI）是基于 Linux 平台的通用型水下机器人控制程序，本项目需要搭配 [ESP-SUB-MASTER](https://github.com/sfxfs/esp-sub-master) 和 [ROV-HOST](https://github.com/bohonghuang/rov-host) 使用。

![](./doc/arch.png)

SUB-NAVI 可以实现以下功能：

- 与 PC 端上位机 (ROV-HOST) 基于 HTTP 协议的 JSON-RPC 2.0 通信（采用 io 监听提高性能）
- 与 SUB-MASTER 基于 Protocol Buffer 的串口通信（用于控制底层外设，推进器，机械臂，等）
- 通过串口外接传感器和其他外设
- 基于文件系统的配置管理（csv、json）
- 运动控制算法

## 编译

本项目搭载于 `Rockchip rv1103`，架构为 `armhf`，请搭配 `arm-rockchip830-linux-uclibcgnueabihf` 编译器使用。

编译使用 Linux，并确保安装：

- 交叉编译器（并添加至 PATH）
- `cmake`

进入项目根目录并输入命令进行编译：

```shell
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-g++ && cmake --build build --config Release
```

如果使用 VSCode 编译，还需安装：

- 对应 CMake 工程编译插件
- `ninja-build`

使用 VSCode 打开项目文件，后在跳出的选框中选择对应工具链，再在底栏选择生成即可编译。

## 开发

项目主要有三个根目录的文件夹

- `3rd` 存放了第三方库和通用的代码
- `config-header` 存放了全局配置头文件
- `user` 存放用户项目源代码

`user` 文件夹下主要有几个功能模块

- `control` 为机器人控制算法
- `csv-json-config-sys` 为程序配置系统代码
- `json-rpc-server` 为 JSON-RPC 服务端应用代码
- `peripheral` 为相关外设驱动
- `protobuf-commu` 为与 SUB-MASTER 通信代码