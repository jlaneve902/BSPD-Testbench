#include "pedal.h"
#include <stdbool.h>

void pedalConstruct(Pedal* p, const char *name, uint32_t thresholdMS){
    p->name = name;
    p->thresholdMS = thresholdMS;
    pedalEnd(p);
}

void pedalStart(Pedal* p){
    p->isPressed = true;
}

void pedalEnd(Pedal* p){
    p->isPressed = false;
    p->overThreshold = false;
    p->timePressedMS = 0;
}

uint32_t pedalReadTime(const Pedal* p){
    return p->timePressedMS;
}

void pedal_tick(Pedal* p, uint32_t deltaMS){
    if(!p->isPressed){
        return;
    }
    if(p->timePressedMS < p->thresholdMS){
        p->timePressedMS += deltaMS;
    }
    p->overThreshold = (p->timePressedMS >= p->thresholdMS);
}

