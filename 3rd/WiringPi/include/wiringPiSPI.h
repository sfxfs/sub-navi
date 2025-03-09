/*
 * wiringPiSPI.h:
 *	Simplified SPI access routines
 *	Copyright (c) 2012-2024 Gordon Henderson and contributors
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://github.com/WiringPi/WiringPi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

int wiringPiSPIDataRW    (const int fd, unsigned char *data, int len) ;
int wiringPiSPISetup     (const char *spidev, int speed, int mode, int bits_per_word, int lsb_first) ;
int wiringPiSPIClose     (const int fd) ;

#ifdef __cplusplus
}
#endif