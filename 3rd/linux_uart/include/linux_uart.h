/*****************************************************************************
* | File        :   navi_uart.h
* | Author      :   luckfox team
* | Function    :   Read and write /dev/UART,  hardware UART
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-08-01
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __LUCKFOX_UART_
#define __LUCKFOX_UART_

#include <stdint.h>
#include <termios.h>

/**
 * Define UART attribute
 **/
typedef struct UARTStruct
{
    int fd;
    struct termios config;
} HARDWARE_UART;

uint8_t navi_uart_begin(HARDWARE_UART *dev, char *UART_device);
uint8_t navi_uart_end(HARDWARE_UART *dev);
uint8_t navi_uart_setBaudrate(HARDWARE_UART *dev, uint32_t Baudrate);

uint8_t navi_uart_write(HARDWARE_UART *dev, const char *buf, uint32_t len);
uint8_t navi_uart_read(HARDWARE_UART *dev, char *buf, uint32_t len);
uint8_t navi_uart_writeByte(HARDWARE_UART *dev, uint8_t buf);
uint8_t navi_uart_readByte(HARDWARE_UART *dev);

uint8_t navi_uart_Set(HARDWARE_UART *dev, int databits, int stopbits, int parity);

#endif
