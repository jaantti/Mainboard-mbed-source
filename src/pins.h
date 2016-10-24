#ifndef PINS_H_
#define PINS_H_
#include "mbed.h"
#include "definitions.h"


PwmOut MOTOR0_PWM(P2_3);
DigitalOut MOTOR0_DIR1(P0_21);
DigitalOut MOTOR0_DIR2(P0_20);
DigitalIn MOTOR0_FAULT(P0_22);
InterruptIn MOTOR0_ENCA(P0_19);
InterruptIn MOTOR0_ENCB(P0_18);

//Testing pins
//PwmOut MOTOR0_PWM(p26);
//DigitalOut MOTOR0_DIR1(p20);
//DigitalOut MOTOR0_DIR2(p19);
//DigitalIn MOTOR0_FAULT(p18);
//InterruptIn MOTOR0_ENCA(p17);
//InterruptIn MOTOR0_ENCB(p16);

PwmOut MOTOR1_PWM(P2_2);
DigitalOut MOTOR1_DIR1(P0_15);
DigitalOut MOTOR1_DIR2(P0_16);
DigitalIn MOTOR1_FAULT(P0_17);
InterruptIn MOTOR1_ENCA(P2_7);
InterruptIn MOTOR1_ENCB(P2_6);
// Testing pins
//InterruptIn MOTOR1_ENCA(p21);
//InterruptIn MOTOR1_ENCB(p22);

PwmOut MOTOR2_PWM(P2_1);
DigitalOut MOTOR2_DIR1(P0_24);
DigitalOut MOTOR2_DIR2(P0_25);
DigitalIn MOTOR2_FAULT(P0_23);
InterruptIn MOTOR2_ENCA(P0_26);
InterruptIn MOTOR2_ENCB(P0_9);

PwmOut MOTOR3_PWM(P2_0);
DigitalOut MOTOR3_DIR1(P0_7);
DigitalOut MOTOR3_DIR2(P0_6);
DigitalIn MOTOR3_FAULT(P0_8);
InterruptIn MOTOR3_ENCA(P0_5);
InterruptIn MOTOR3_ENCB(P0_4);

PwmOut* MotorPwm[] = {
    &MOTOR0_PWM,
    &MOTOR1_PWM,
    &MOTOR2_PWM,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_PWM
    #endif
};

DigitalOut* MotorDir1[] = {
    &MOTOR0_DIR1,
    &MOTOR1_DIR1,
    &MOTOR2_DIR1,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_DIR1
    #endif
};

DigitalOut* MotorDir2[] = {
    &MOTOR0_DIR2,
    &MOTOR1_DIR2,
    &MOTOR2_DIR2,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_DIR2
    #endif
};

DigitalIn* MotorFault[] = {
    &MOTOR0_FAULT,
    &MOTOR1_FAULT,
    &MOTOR2_FAULT,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_FAULT
    #endif
};

InterruptIn* MotorEncA[] = {
    &MOTOR0_ENCA,
    &MOTOR1_ENCA,
    &MOTOR2_ENCA,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_ENCA
    #endif
};

InterruptIn* MotorEncB[] = {
    &MOTOR0_ENCB,
    &MOTOR1_ENCB,
    &MOTOR2_ENCB,
    #if NUMBER_OF_MOTORS == 4
    &MOTOR3_ENCB
    #endif
};


#endif
