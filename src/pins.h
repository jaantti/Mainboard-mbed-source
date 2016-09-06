#ifndef PINS_H_
#define PINS_H_
#include "mbed.h"
#include "definitions.h"

PwmOut MOTOR0_PWM(p26);
DigitalOut MOTOR0_DIR1(p20);
DigitalOut MOTOR0_DIR2(p19);
DigitalIn MOTOR0_FAULT(p18);
InterruptIn MOTOR0_ENCA(p17);
InterruptIn MOTOR0_ENCB(p16);

PwmOut MOTOR1_PWM(P2_3);
DigitalOut MOTOR1_DIR1(P0_21);
DigitalOut MOTOR1_DIR2(P0_20);
DigitalIn MOTOR1_FAULT(P0_22);
InterruptIn MOTOR1_ENCA(P0_19);
InterruptIn MOTOR1_ENCB(P0_18);

#if (NUMBER_OF_MOTORS == 1)
    PwmOut* MotorPwm[] = {&MOTOR0_PWM, &MOTOR1_PWM};
    DigitalOut* MotorDir1[] = {&MOTOR0_DIR1, &MOTOR1_DIR1};
    DigitalOut* MotorDir2[] = {&MOTOR0_DIR2, &MOTOR1_DIR2};
    DigitalIn* MotorFault[] = {&MOTOR0_FAULT, &MOTOR1_FAULT};
    InterruptIn* MotorEncA[] = {&MOTOR0_ENCA, &MOTOR1_ENCA};
    InterruptIn* MotorEncB[] = {&MOTOR0_ENCB, &MOTOR1_ENCB};
#endif

#if (NUMBER_OF_MOTORS == 4)
    /* code */
#endif


#endif
