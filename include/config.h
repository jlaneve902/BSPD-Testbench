/*Macros all set in this one header file for better organization
of each macro value and ease of reuse for main*/


#ifndef CONFIG_H
#define CONFIG_H

//Threshold and Tick Values
#define BSPD_THRESHOLD_MS 500
#define TICK_PERIOD_MS 1

//Min and Max Voltages
#define MAX_VOLTAGE 4.5f
#define MIN_VOLTAGE 0.5f

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

