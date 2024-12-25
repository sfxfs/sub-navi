/*****************************************************************************
* | File        :   navi_uart.c
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

#define TAG "uart"

#include "linux_uart.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "log.h"

/******************************************************************************
function:   Serial port initialization
parameter:
    UART_device : Device name
Info:
    /dev/ttyTHS*
    The default baud rate is 9600, 8-bit data, 1 stop bit, no parity
******************************************************************************/
uint8_t navi_uart_begin(HARDWARE_UART *dev, const char *UART_device)
{
    // device
    if ((dev->fd = open(UART_device, O_RDWR | O_NOCTTY)) < 0)
    { // 打开UART
        log_debug("open failed: %s", UART_device);
        return 1;
    }
    else
    {
        log_debug("open: %s", UART_device);
    }
    navi_uart_setBaudrate(dev, 115200);
    navi_uart_Set(dev, 8, 1, 'N');
    return 0;
}

/******************************************************************************
function:   Set the serial port baud rate
parameter:
    Baudrate : Serial port baud rate
               B50          50
               B75          75
               B110         110
               B134         134
               B150         150
               B200         200
               B300         300
               B600         600
               B1200        1200
               B1800        1800
               B2400        2400
               B4800        4800
               B9600        9600
               B19200       19200
               B38400       38400
               B57600       57600
               B115200      115200
               B230400      230400
Info:
******************************************************************************/
uint8_t navi_uart_setBaudrate(HARDWARE_UART *dev, uint32_t Baudrate)
{
    uint16_t err;
    uint32_t baud;
    tcflush(dev->fd, TCIOFLUSH); // 清空数据缓冲区

    switch (Baudrate) // Set the number of data bits
    {
    case B230400:case 230400:baud = B230400;break;
    case B115200:case 115200:baud = B115200;break;
    case B57600:case 57600:baud = B57600;break;
    case B38400:case 38400:baud = B38400;break;
    case B19200:case 19200:baud = B19200;break;
    case B9600:case 9600:baud = B9600;break;
    case B4800:case 4800:baud = B4800;break;
    case B2400:case 2400:baud = B2400;break;
    case B1800:case 1800:baud = B1800;break;
    case B1200:case 1200:baud = B1200;break;
    case B600:case 600:baud = B600;break;
    case B300:case 300:baud = B300;break;
    default:baud = B9600;break;
    }
    log_debug("Baud rate setting");
    if (cfsetispeed(&dev->config, baud) != 0)
    {
        log_error("Baud rate setting failed 1");
        return 1;
    }
    if (cfsetospeed(&dev->config, baud) != 0)
    {
        log_error("Baud rate setting failed 2");
        return 1;
    }
    err = tcsetattr(dev->fd, TCSANOW, &dev->config);
    if (err != 0)
    {
        log_error("Setting the terminal baud rate failed");
        return 1;
    }
    log_debug("Baud rate set to %ld", Baudrate);
    return 0;
}

int navi_uart_data_avail(HARDWARE_UART dev)
{
    int result;

    if (ioctl(dev.fd, FIONREAD, &result) == -1)
        return -1;

    return result;
}

/******************************************************************************
function: Set serial port parameters
parameter:
    databits :   Data bit
    stopbits :   Stop bit
    parity   :   Parity bit
Info:
******************************************************************************/
uint8_t navi_uart_Set(HARDWARE_UART *dev, int databits, int stopbits, int parity)
{
    if (tcgetattr(dev->fd, &dev->config) != 0)
    {
        log_error("Failed to get terminal parameters");
        return 0;
    }
    dev->config.c_cflag |= (CLOCAL | CREAD); // Generally set flag

    switch (databits) // Set the number of data bits
    {
    case 5:
        dev->config.c_cflag &= ~CSIZE;
        dev->config.c_cflag |= CS5;
        break;
    case 6:
        dev->config.c_cflag &= ~CSIZE;
        dev->config.c_cflag |= CS6;
        break;
    case 7:
        dev->config.c_cflag &= ~CSIZE;
        dev->config.c_cflag |= CS7;
        break;
    case 8:
        dev->config.c_cflag &= ~CSIZE;
        dev->config.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unsupported data size.\n");
        return 0;
    }

    switch (parity) // Set check digit
    {
    case 'n':case 'N':
        dev->config.c_cflag &= ~PARENB; // Clear check digit
        dev->config.c_iflag &= ~INPCK;  // enable parity checking
        break;
    case 'o':case 'O':
        dev->config.c_cflag |= PARENB; // enable parity
        dev->config.c_cflag |= PARODD; // Odd parity
        dev->config.c_iflag |= INPCK;  // disable parity checking
        break;
    case 'e':case 'E':
        dev->config.c_cflag |= PARENB;  // enable parity
        dev->config.c_cflag &= ~PARODD; // Even parity
        dev->config.c_iflag |= INPCK;   // disable pairty checking
        break;
    case 's':case 'S':
        dev->config.c_cflag &= ~PARENB; // Clear check digit
        dev->config.c_cflag &= ~CSTOPB;
        dev->config.c_iflag |= INPCK; // disable pairty checking
        break;
    default:
        fprintf(stderr, "Unsupported parity.\n");
        return 0;
    }
    switch (stopbits) // Set stop bit  1   2
    {
    case 1:
        dev->config.c_cflag &= ~CSTOPB;
        break;
    case 2:
        dev->config.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr, "Unsupported stopbits.\n");
        return 0;
    }
    dev->config.c_cflag |= (CLOCAL | CREAD);
    dev->config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    dev->config.c_oflag &= ~OPOST;
    dev->config.c_oflag &= ~(ONLCR | OCRNL);
    dev->config.c_iflag &= ~(ICRNL | INLCR);
    dev->config.c_iflag &= ~(IXON | IXOFF | IXANY);
    tcflush(dev->fd, TCIFLUSH);
    dev->config.c_cc[VTIME] = 150;                      // Set timeout
    dev->config.c_cc[VMIN] = 1;                         //
    if (tcsetattr(dev->fd, TCSANOW, &dev->config) != 0) // Update the dev->config and do it now
    {
        log_error("Setting terminal parameters failed");
        return 0;
    }
    log_debug("Set terminal parameters successfully");
    return 1;
}
