#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "pedal.h"
#include "signals.h"
#include "parser.h"

static_assert(configTICK_RATE_HZ == 1000, "FreeRTOS tick must be 1000 Hz");

//Safety so no outside c files can affect pedal values.
static Pedal throttle;
static Pedal brake;
static Signal throttlePWM;
static Signal brakePWM;


static volatile bool useSerial = false;
static volatile bool throttleHeld = false;
static volatile bool brakeHeld = false;
static volatile bool faultLatched = false;

static bool bspdCheck(){
    bool throttleOver = (throttlePWM.currVoltage >= THROTTLE_THRESHOLD_MV);
    bool brakeOver = (brakePWM.currVoltage >= BRAKE_THRESHOLD_MV);

    if(throttleOver && !throttle.isPressed){pedalStart(&throttle);}

    if(!throttleOver && throttle.isPressed){pedalEnd(&throttle);}

    if(brakeOver && !brake.isPressed){pedalStart(&brake);}

    if(!brakeOver && brake.isPressed){pedalEnd(&brake);}

    pedal_tick(&throttle, TICK_PERIOD_MS);
    pedal_tick(&brake, TICK_PERIOD_MS);

    if(throttle.overThreshold && brake.overThreshold){
        signalShutdown(&throttlePWM);
        signalShutdown(&brakePWM);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, HIGH);
        return true;
    } 

    digitalWrite(LED_GREEN, throttle.overThreshold);
    digitalWrite(LED_YELLOW, brake.overThreshold);

    return false;
}

void bspdTask(void *pvParamters){
    TickType_t lastWake = xTaskGetTickCount();
    
    for(;;){
        if(useSerial){
            signalMoveToTarget(&throttlePWM);
            signalMoveToTarget(&brakePWM);
        }else{
            signalMove(&throttlePWM, throttleHeld);
            signalMove(&brakePWM, brakeHeld);
        }
        signalWrite(&throttlePWM);
        signalWrite(&brakePWM);

        if(bspdCheck()){
            faultLatched = true;
            break;
        }     
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(TICK_PERIOD_MS));
    }
    
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void buttonTask(void *pvParameters){
    for(;;){
        throttleHeld = (digitalRead(THROTTLE_PIN) == THROTTLE_PRESSED_LEVEL);
        brakeHeld = (digitalRead(BRAKE_PIN) == BRAKE_PRESSED_LEVEL);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void handleLine(char* line){
    command cmd = parseCommand(line);

    switch(cmd.type){
        case COMMAND_NONE:
            break;
        case COMMAND_ERROR:
            Serial.println("Bad Command Pass");
            break;
        case COMMAND_SET_APPS:
        case COMMAND_SET_BRAKE:
            if(faultLatched){
                Serial.println("Error fault latched");
                break;
            }
            signalSetPercent(cmd.type == COMMAND_SET_APPS ? &throttlePWM : &brakePWM, cmd.percent);
            Serial.println("Good");
            break;
    }
}

void serialTask(void *pvParameters){
    char line[SERIAL_LINE_MAX];
    size_t len = 0;
    bool faultReported = false;

    for(;;){
        while(Serial.available() > 0){
            char c = (char)Serial.read();
            if(c == '\n' || c == '\r'){
                if(len > 0){
                    line[len] = '\0';
                    handleLine(line);
                    len = 0;
                }
            }else if(len < SERIAL_LINE_MAX - 1){
                line[len++] = c;
            }
        }

        if(faultLatched && !faultReported){
            Serial.println("FAULT: both pedals over threshold output now off.");
            faultReported = true;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

//Parses user input to decide whether using button or serial
static bool chooseMode(){
    char buff[16];
    size_t len = 0;
    unsigned long lastPrompt = 0;
    bool firstPrompt = true;

    for(;;){
        if(firstPrompt || millis() - lastPrompt >= 2000){
            Serial.println("BUTTON or SERIAL");
            lastPrompt = millis();
            firstPrompt = false;
        }

        while(Serial.available() > 0){
            char c = (char)Serial.read();
            if(c == '\n' || c == '\r'){
                if(len > 0){
                    buff[len] = '\0';
                    for(size_t i = 0; i < len; i++){
                        buff[i] = (char)toupper((unsigned char) buff[i]);
                    }
                    if(strcmp(buff, "SERIAL") == 0) return true;
                    if(strcmp(buff, "BUTTON") == 0) return false;
                    Serial.println("Retry, message unknown.");
                    len = 0;
                }
            }else if(len < sizeof(buff) -1){
                buff[len++] = c;
            }
        }
        delay(10);
    }
}

void setup(){

    Serial.begin(SERIAL_BAUD);
    Serial.println("DEBUG: setup started");

    //Breadboard configurations

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);  

    //Pedal Setup
    pedalConstruct(&throttle, "THROTTLE", BSPD_THRESHOLD_MS);
    pedalConstruct(&brake, "BRAKE", BSPD_THRESHOLD_MS);

    //PWM Setup
    signalConstruct(&throttlePWM, THROTTLE_PWM_PIN);
    signalConstruct(&brakePWM, BRAKE_PWM_PIN);

    useSerial = chooseMode();

    //Creates separate thread for freeRTOS to run. 
    xTaskCreate(bspdTask, "bspd", 256, NULL, 2, NULL);

    if(useSerial){
        Serial.println("Serial Mode. APPS or BRAKE as commands.");
        xTaskCreate(serialTask, "serial", 512, NULL, 1, NULL);
    }else{
        Serial.println("Button Mode.");
        pinMode(THROTTLE_PIN ,INPUT);
        pinMode(BRAKE_PIN, INPUT);
        xTaskCreate(buttonTask, "button", 256, NULL, 1, NULL);
    }

    //Starts freeRTOS
    vTaskStartScheduler();
}

void loop(){}