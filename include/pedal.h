#ifndef PEDAL_H
#define PEDAL_H

#include <stdint.h>
#include <stdbool.h>

//cpp compiler knows how to compile and link c functions with cpp file
#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
    const char* name;
    volatile bool isPressed;
    volatile bool overThreshold;
    volatile uint32_t timePressedMS;
    uint32_t thresholdMS;

}Pedal;

/**
 * Constructor for struct
 */
void pedalConstruct(Pedal* p, const char* name, uint32_t overThreshold);

/**
 * Pedal has been pressed, stopwatch begins.
 */
void pedalStart(Pedal* p);

/**
 * Pedal no longer pressed, stopwatch ends.
 */
void pedalEnd(Pedal* p);

/**
 * Adding time to stopwatch for each pedal, caps at 500ms.
 */
void pedal_tick(Pedal *p, uint32_t deltaMS);

#ifdef __cplusplus
}
#endif

#endif