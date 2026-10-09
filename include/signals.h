#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdint.h>
#include <stdbool.h>

//cpp compiler knows how to compile and link c functions with cpp file
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    uint32_t currVoltage;
    uint16_t pwmPin;
}Signal;

/**
 * Constructor for signal struct
 */
void signalConstruct(Signal* s, uint16_t pin);

/**
 * Method to either increment or decrement currVoltage value based on if pedal is pressed or not
 */
void signalMove(Signal* s, bool pressed);

/**
 * Writes an adjusted value between 0-255 for PWM output
 */
void signalWrite(const Signal* s);

/**
 * Fault has been detected (both pedals 500ms), shuts off PWM
 */
void signalShutdown(Signal* s);


#ifdef __cplusplus
}
#endif
#endif