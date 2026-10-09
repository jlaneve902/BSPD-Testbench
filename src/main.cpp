#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include "config.h"
#include "pedal.h"

static_assert(configTICK_RATE_HZ == 1000, "FreeRTOS tick must be 1000 Hz");

//Safety so no outside c files can affect pedal values.
static Pedal throttle;
static Pedal brake;
static volatile bool faultLatched = false;

void bspdTask(void *pvParamters){
    TickType_t lastWake = xTaskGetTickCount();

    //Infinite loop setup
    for(;;){
        bool throttleNow = (digitalRead(THROTTLE_PIN) == THROTTLE_PRESSED_LEVEL);
        bool brakeNow = (digitalRead(BRAKE_PIN) == BRAKE_PRESSED_LEVEL);

        if(throttleNow && !throttle.isPressed){pedalStart(&throttle);}
        if(!throttleNow && throttle.isPressed){pedalEnd(&throttle);}
        if(brakeNow && !brake.isPressed){pedalStart(&brake);}
        if(!brakeNow && brake.isPressed){pedalEnd(&brake);}

        pedal_tick(&throttle, TICK_PERIOD_MS);
        pedal_tick(&brake, TICK_PERIOD_MS);

        if(throttle.overThreshold && brake.overThreshold){
            faultLatched = true;
        }

        digitalWrite(LED_GREEN, throttle.overThreshold);
        digitalWrite(LED_YELLOW, brake.overThreshold);
        digitalWrite(LED_RED, faultLatched);
        
        //Makes sure ticks start at ms intervals
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(TICK_PERIOD_MS));
    }
}

void setup(){
    //Breadboard configurations
    pinMode(THROTTLE_PIN ,INPUT);
    pinMode(BRAKE_PIN, INPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);  

    //Pedal Setup
    pedalConstruct(&throttle, "THROTTLE", BSPD_THRESHOLD_MS);
    pedalConstruct(&brake, "BRAKE", BSPD_THRESHOLD_MS);

    //Creates separate thread for freeRTOS to run.
    xTaskCreate(bspdTask, "bspd", 256, NULL, 2, NULL);

    //Starts freeRTOS
    vTaskStartScheduler();
}

void loop(){}