//
// Created by cocon on 16.01.2026.
//

#ifndef ARDUINO_ABSTRACT_TIMER_H
#define ARDUINO_ABSTRACT_TIMER_H

#include <assert.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#endif //ARDUINO_ABSTRACT_TIMER_H

// triggers on TCNT1 == OCR1A


/**
 * will expand to be more versatile currently only inits timer1 with prescaler x8 in ClC mode
 * @param ticks
 */
static inline void initTimer1(int ticks) {
    assert(ticks > 0);
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    // no prescaler
    TCCR1B |= (1 << WGM12) | (1 << CS11);
    TCCR1B &=  ~(1 << CS12) | (1 << CS10);
    // when TCNT1 == OCR1A it fires the intr
    OCR1A = ticks - 1;
    // allow Timer1 to trigger
    TIMSK1 |= (1 << OCIE1A);
}

