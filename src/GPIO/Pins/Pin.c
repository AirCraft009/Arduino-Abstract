//
// Created by cocon on 28.12.2025.
//
#include "Pin.h"
#include <avr/interrupt.h>

/**
 *
 * Sets Pins and returns information in struct
 *
 * @param PinNumber Number from 0-13 like on the Arduino
 * @param mode Set to Write(true) or Read(false)
 * @return A Pin Struct containg this information
 */
int SetPin(Pin *pin, int PinNumber, bool mode) {
    if (CHECK_PIN_VALIDITY(PinNumber)) return 1;

    pin->dataDirRegPtr = GET_DATA_DIRECTION_REGISTER(PinNumber);
    pin->PinNumber = (PinNumber <= 7 ? PinNumber : PinNumber - 8);
    pin->mode = mode;

    if (mode) {
        SET_DATA_DIRECTION_WRITE(*pin->dataDirRegPtr, pin->PinNumber);
        pin->portPtr = CONVERT_NUM_TO_PORT(PinNumber);
    }
    else {
        SET_DATA_DIRECTION_READ(*pin->dataDirRegPtr, pin->PinNumber);
        pin->portPtr = CONVERT_NUM_TO_PIN(PinNumber);
    }
    return 0;
}



int DigitalWrite(const Pin *p, int value) {
    if (!p || !p->mode) return 1;

    if (value != LOW)
        *p->portPtr |=  (1 << p->PinNumber);
    else
        *p->portPtr &= ~(1 << p->PinNumber);

    return 0;
}

int DigitalRead(const Pin *p) {
    if (!p || p->mode) return 1;

    uint8_t val = (*p->portPtr & (1 << p->PinNumber)) == 0;
    return val;
}

void InitPCINT(const Pin *p) {
    // set as input
    SET_DATA_DIRECTION_READ(*p->dataDirRegPtr, p->PinNumber);
    SET_PULL_DOWN(*p->portPtr, p->PinNumber);
    PCICR |= (1 << GET_PCIE_BIT(*p->portPtr));
    volatile uint8_t* mask = (GET_INTERRUPT_MASK(*p->portPtr));
    // TODO: replace p.PinNumber with translated PCINTn so that PDn works
    *mask |= (1 << p->PinNumber);
}