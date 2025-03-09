#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "wiringPiSPI.h"

int wiringPiSPISetup(const char *spidev, int speed, int mode, int bits_per_word, int lsb_first)
{
    int fd = open(spidev, O_RDWR);
    if (fd < 0)
    {
        printf("Failed to open SPI device: %s\n", spidev);
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        printf("Failed to set SPI mode: %s\n", spidev);
        return -2;
    }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0)
    {
        printf("Failed to set SPI bits per word: %s\n", spidev);
        return -3;
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("Failed to set SPI speed: %s\n", spidev);
        return -4;
    }
    if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb_first) < 0)
    {
        printf("Failed to set SPI LSB first: %s\n", spidev);
        return -5;
    }

    return fd;
}

int wiringPiSPIClose(const int fd)
{
    return close(fd);
}

int wiringPiSPIDataRW(const int fd, unsigned char *data, int len)
{
    struct spi_ioc_transfer spi;
    memset(&spi, 0, sizeof(spi));

    spi.tx_buf = (unsigned long)data;
    spi.rx_buf = (unsigned long)data;
    spi.len = len;

    return ioctl(fd, SPI_IOC_MESSAGE(1), &spi);
}
