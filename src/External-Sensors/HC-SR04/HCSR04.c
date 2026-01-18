#include "HCSR04.h"
#include "../../Timers/timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint64_t time_mircoS = 0;
Pin sig = EMPTY_PIN;

void setPins(int trigPin, int echoPin, HC_SR04 hcSR04) {
    // currently just accepts all pins but will control in future

    SetPin(&hcSR04.Trig, trigPin, true);
    SetPin(&hcSR04.Echo, echoPin, false);
    InitPCINT(&hcSR04.Echo);
    initTimer1(2);
}

/**
 *
 * @param trigPin GPIO pin
 * @param echoPin GPIO pin
 * @return -1 if unable to register sensor due to too many being in use else position of the sensor
 */
int registerHC_SR04(int trigPin, int echoPin) {
    if (hc_sr04Ptr >= MaxSensors) {
        return -1;
    }
    HC_SR04 hc_sr04 = EMPTY_HC_SR04;
    setPins(trigPin, echoPin, hc_sr04);
    hc_sr04Sensors[hc_sr04Ptr] = hc_sr04;
    hc_sr04Ptr++;
    return hc_sr04Ptr-1;
}

// triggers on TCNT1 == OCR1A
ISR(TIMER1_COMPA_vect) {
    time_mircoS ++;
}


uint64_t micros() {
    uint32_t t;
    cli();
    t = time_mircoS;
    sei();
    return t;
}

/**
 *
 * @param index index of the sensor passed back from RegisterHc_SR04
 * @return 1 - invalid index
 */
int trigger(int index) {
    if (index >= hc_sr04Ptr) {
        return 1;
    }

    HC_SR04 hc_sr04 = hc_sr04Sensors[index];
    hc_sr04.done = false;
    DigitalWrite(&hc_sr04.Trig, HIGH);
    _delay_us(10);
    DigitalWrite(&hc_sr04.Trig, LOW);
    hc_sr04.measureStart = micros();
    return 0;
}


// ISR for PORTD == PORTB
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT0_vect) {

    for (int i = 0; i < hc_sr04Ptr; i++) {
        HC_SR04 hc_sr04 = hc_sr04Sensors[i];
        if (hc_sr04.done) {
            hc_sr04.measureEnd = micros();
            hc_sr04.done = false;
        }
    }
    DigitalWrite(&sig, LOW);
}

uint64_t measure(int index) {
    sei();
    if (trigger(index) == 1) {
        return 0;
    }
    //make measure blocking
    return GET_CM_FROM_US(hc_sr04Sensors[index].measureEnd - hc_sr04Sensors[index].measureStart);
}