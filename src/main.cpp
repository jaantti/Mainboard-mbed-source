#include "mbed.h"
#include "pins.h"
#include "motor.h"

DigitalOut led(LED1);
Serial pc(USBTX, USBRX);

Motor m1(&pc, &MOTOR1_PWM, &MOTOR1_DIR1, &MOTOR1_DIR2, &MOTOR1_FAULT, &MOTOR1_ENCA, &MOTOR1_ENCB);

int main() {
    pc.printf("Start\n");
    m1.setSpeed(100);
    while(1) {
        m1.pid();
        led = 1;
        wait(0.2);
        led = 0;
        wait(0.2);
        pc.printf("Loop\n");
    }
}
