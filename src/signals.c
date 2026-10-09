#include <Arduino.h>
#include "signals.h"
#include "config.h"


void signalConstruct(Signal* s, uint16_t pin){
    s->pwmPin = pin;
    s->currVoltage = MIN_VOLTAGE_MV;
    pinMode(pin, OUTPUT);
}

void signalMove(Signal* s, bool pressed){
    if(pressed){
        if(s->currVoltage += V_CHANGE_SIZE_MV >= MAX_VOLTAGE_MV) {s->currVoltage = MAX_VOLTAGE_MV;}
        else {s->currVoltage += V_CHANGE_SIZE_MV;}
    }else{
        if(s->currVoltage -= V_CHANGE_SIZE_MV <= MIN_VOLTAGE_MV) {s->currVoltage = MIN_VOLTAGE_MV;}
        else{s->currVoltage -= V_CHANGE_SIZE_MV;}
    }
}

void signalWrite(const Signal* s){

    //Adjusting 0.5-4.5 V range to fit into the 0 - 255 PWM 8 bit range
    uint32_t duty = (s->currVoltage * PWM_MAX) / MAX_VOLTAGE_MV;

    analogWrite(s->pwmPin, duty);
}

void signalShutdown(Signal* s){
    s->currVoltage = 0;
    analogWrite(s->pwmPin, 0);
}