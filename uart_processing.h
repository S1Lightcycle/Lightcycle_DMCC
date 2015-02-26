/*
 * uart_processing.h
 *
 *  Created on: Jan 8, 2015
 *      Author: woot
 */

#ifndef UART_PROCESSING_H_
#define UART_PROCESSING_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "lc_protocol.h"
#include <unistd.h>
#include <string.h>

void initUART();
lc_protocol_t getCommand();
void closeUART();

#endif /* UART_PROCESSING_H_ */
