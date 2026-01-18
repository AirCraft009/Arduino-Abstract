//
// Created by cocon on 16.01.2026.
//

#include <util/delay.h>

#include "GPIO/Serial/Serial.h"

int main(void)
{
    uart_init_err err = InitSerial(9600, PARITY_DISABLE, 8, 1, TRANSMITTER_RECEIVER);
    if (err != OK) {
        return 1;
    }
    putCConsole('a');
    Serial_flush();
    while (1) {
        putCConsole(readC());
    }
}