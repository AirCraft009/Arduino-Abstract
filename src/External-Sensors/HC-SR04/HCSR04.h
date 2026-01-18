#ifndef HC_SR04_LIBRARY_H
#define HC_SR04_LIBRARY_H
#include "../../GPIO/Pins/Pin.h"

#define GET_CM_FROM_US(t) (t/58)
#define GET_IN_FROM_US(t) (t/148)
#define MaxSensors 4
#define EMPTY_HC_SR04 {EMPTY_PIN, EMPTY_PIN, 0, 0, false}

#endif // HC_SR04_LIBRARY_H

int sendPulse(void);

typedef struct HC_SR04 {
    Pin Trig;
    Pin Echo;
    // in mikrosec
    uint64_t measureStart;
    uint64_t measureEnd;
    bool done;
}HC_SR04;

static int hc_sr04Ptr = 0;
static HC_SR04 hc_sr04Sensors[MaxSensors];


