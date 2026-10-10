# BSPD-Testbench
Bench signal generator for testing a brake system plausibility device (BSPD). It outputs simulated brake pressure and APPS voltages (0.5 V to 4.5 V) and shuts everything off if both pedals are held over threshold for 500 ms. Runs on an STM32 Nucleo-H563ZI with FreeRTOS, built with PlatformIO.

#Hardware
Nucleo-H563ZI
Dual rail-to-rail op amp, one channel per pedal, powered from 5 V and GND
RC filter per channel: 10k resistor and 0.47 uF capacitor
Op amp gain resistors per channel: 3.6k (Rf) and 10k (Rg)
Pushbutton with a 10k pull-down for the brake input
Multimeter

#Pins
Throttle button    USER_BTN (blue on-board button)
Brake button       D2
Throttle PWM out   D5
Brake PWM out      D6
Green LED          LED_GREEN  (throttle over threshold)
Yellow LED         LED_YELLOW (brake over threshold)
Red LED            LED_RED    (fault)
