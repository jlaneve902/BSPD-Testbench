/*Macros all set in this one header file for better organization
of each macro value and ease of reuse for main*/

#ifndef CONFIG_H
#define CONFIG_H

//Threshold and Tick Values
#define BSPD_THRESHOLD_MS 500
#define TICK_PERIOD_MS 1

#define THROTTLE_THRESHOLD_MV 1500
#define BRAKE_THRESHOLD_MV    1500

//Min and Max Voltages + Step Size for PWM
#define MAX_VOLTAGE_MV 4500
#define MIN_VOLTAGE_MV 500
#define V_CHANGE_SIZE_MV 8

//Serial Settings
#define SERIAL_BAUD 115200
#define SERIAL_LINE_MAX 64

//PWM Min and Max for STM32
#define PWM_MAX 255
#define PWM_MIN 0

//PWM Pin Locations
#define BRAKE_PWM_PIN D6
#define THROTTLE_PWM_PIN D5

//Throttle Pin Information
#define THROTTLE_PIN USER_BTN
#define THROTTLE_PRESSED_LEVEL HIGH

//Brake Pin Information
#define BRAKE_PIN D2
#define BRAKE_PRESSED_LEVEL HIGH

#endif

