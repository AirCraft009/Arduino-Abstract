//
// Created by cocon on 16.01.2026.
//

#ifndef ARDUINO_ABSTRACT_SERIAL_H
#define ARDUINO_ABSTRACT_SERIAL_H

#include "../Pins/Pin.h"
#include "avr/io.h"
#include "../../Timers/timer.h"

#define ODD_PARITY 3
#define EVEN_PARITY 2
#define PARITY_DISABLE 0
#define TRANSMITTER 1
#define RECEIVER 2
#define TRANSMITTER_RECEIVER 3


typedef enum {
    OK = 0,
    Invalid_DataBits = 1,
    Invalid_baud = 2,
    Invalid_parity = 3,
    Invalid_stopBits = 4,
    Invalid_enable = 5,
} uart_init_err;
#endif //ARDUINO_ABSTRACT_SERIAL_H


uart_init_err InitSerial(uint32_t baud, uint8_t pairity, uint8_t dataBits, uint8_t stopBits, uint8_t enable);
void putC(unsigned char data);
void putStr(uint8_t * str);
uint8_t readC(void);
void read(uint8_t * buf, uint16_t len);
void putCConsole(uint8_t data);
void Serial_flush(void);
void uart_init_minimal(void);