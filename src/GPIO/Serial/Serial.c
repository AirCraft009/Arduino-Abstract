//
// Created by cocon on 16.01.2026.
#include "Serial.h"

#include <util/delay.h>

#define GET_UBRR(baud) ((F_CPU / (16UL * baud)) - 1)
#define SERIAL_SET_PARITY_BITS(parity) (\
    (UCSR0C |= ((parity & 0x01) << UPM00) | (((parity >> 1) & 0x01) << UPM00))\
)
#define SERIAL_SET_DATA_BITS(bits) (\
    (UCSR0C |= (((bits - 5) & 0x01) << UCSZ00 ) | (((bits - 5) >> 1 & 0x01) << UCSZ01))\
)


/**
 *
 *  Initalizes the Serial connection with the specified parameters \n
 *  !! It also diables interrupts globally!!
 *
 * @param baud baud rate
 * @param pairity pairity none, even or odd
 * @param dataBits ammount of bits per char (5 - 8)
 * @param stopBits ammount of stop bits (1 - 2)
 * @param enable transmitter, receiver or both
 * @return
 */
uart_init_err InitSerial(uint32_t baud, uint8_t pairity, uint8_t dataBits, uint8_t stopBits, uint8_t enable) {
    sei();
    if (baud == 0) {
        return Invalid_baud;
    }
    if (pairity > ODD_PARITY) {
        return Invalid_parity;
    }
    if (dataBits < 5 || dataBits > 8) {
        return Invalid_DataBits;
    }
    if (stopBits < 1 || stopBits > 2) {
        return Invalid_stopBits;
    }
    if (enable > TRANSMITTER_RECEIVER) {
        return Invalid_enable;
    }
    uint16_t ubrr = GET_UBRR(baud);

    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;

    UCSR0C = 0;
    UCSR0B = 0;

    UCSR0B = ((enable & 0x1) <<RXEN0) | (((enable >> 1) & 0x1) << TXEN0);
    SERIAL_SET_DATA_BITS(dataBits);
    SERIAL_SET_PARITY_BITS(pairity);
    Serial_flush();
    return OK;
}


void putC(uint8_t data) {
    while (!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

void putCConsole(uint8_t data) {
    if (data == '\n') {
        putC('\r');
    }else if (data == '\r') {
        putC('\n');
        putC(data);
        return;
    }
    putC(data);
}

void putStr(uint8_t * str) {
    while (*str) {
        putC(*str++);
    }
}

uint8_t readC(void) {
    while (!(UCSR0A & (1<<RXC0)))
        ;
    return UDR0;
}


void Serial_flush(void) {
    uint8_t dummy;
    while (UCSR0A & (1 << RXC0)) {
        dummy = UDR0;
    }
}

/**
 * read data into a buffer
 */
void read(uint8_t * buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        buf[i] = readC();
    }
}