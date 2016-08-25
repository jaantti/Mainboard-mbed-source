#ifndef PINS_H_
#define PINS_H_
#include "mbed.h"
PwmOut MOTOR1_PWM(P2_3);
DigitalOut MOTOR1_DIR1(P0_21);
DigitalOut MOTOR1_DIR2(P0_20);
DigitalIn MOTOR1_FAULT(P0_22);
InterruptIn MOTOR1_ENCA(P0_19);
InterruptIn MOTOR1_ENCB(P0_18);
#endif
