#include "mbed.h"
#include "pins.h"
#include "motor.h"

DigitalOut led(LED1);
DigitalOut l(LED2);
Serial pc(USBTX, USBRX);
char buf[16];
bool serialData = false;
int serialCount = 0;

Motor m1(&pc, &MOTOR1_PWM, &MOTOR1_DIR1, &MOTOR1_DIR2, &MOTOR1_FAULT, &MOTOR1_ENCA, &MOTOR1_ENCB);

void serialInterrupt();
void parseCommad(char *command);

int main() {
    pc.printf("Start\n");
    m1.setSpeed(100);
    pc.attach(&serialInterrupt);

    while(1) {
        if (serialData) {
            char temp[16];
            memcpy(temp, buf, 16);
            memset(buf, 0, 16);
            serialData = false;
            pc.printf("temp: %s", temp);
            parseCommad(temp);
        }
        m1.pid();
        led = 1;
        wait(0.2);
        led = 0;
        wait(0.2);
        pc.printf("buf: %s\n", buf);
        //pc.printf("Loop\n");
    }
}

void serialInterrupt(){
    while(pc.readable()) {
        buf[serialCount] = pc.getc();
        serialCount++;
    }
    if (buf[serialCount - 1] == '\n') {
        serialData = true;
        serialCount = 0;
    }
}

void parseCommad (char *command) {

}
