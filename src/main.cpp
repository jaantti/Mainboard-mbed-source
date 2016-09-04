#include "mbed.h"
#include "pins.h"
#include "motor.h"

const int numberOfMotors = 3;

DigitalOut led(LED1);
DigitalOut l(LED2);
Serial pc(USBTX, USBRX);

Ticker motor0PidTicker;

char buf[16];
bool serialData = false;
int serialCount = 0;

volatile int16_t motor0Ticks = 0;
volatile uint8_t motor0EncNow = 0;
volatile uint8_t motor0EncLast = 0;

//Motor m1(&pc, &MOTOR1_PWM, &MOTOR1_DIR1, &MOTOR1_DIR2, &MOTOR1_FAULT, &MOTOR1_ENCA, &MOTOR1_ENCB);
Motor motors[numberOfMotors];

void serialInterrupt();
void parseCommad(char *command);
void motor0EncTick();
void motor0PidTick();

int main() {
    MOTOR0_ENCA.mode(PullNone);
    MOTOR0_ENCB.mode(PullNone);

    motors[0] = Motor(&pc, &MOTOR0_PWM, &MOTOR0_DIR1, &MOTOR0_DIR2, &MOTOR0_FAULT, &MOTOR0_ENCA, &MOTOR0_ENCB, &led);
    MOTOR0_ENCA.rise(&motor0EncTick);
    MOTOR0_ENCA.fall(&motor0EncTick);
    MOTOR0_ENCB.rise(&motor0EncTick);
    MOTOR0_ENCB.fall(&motor0EncTick);

    motor0PidTicker.attach(&motor0PidTick, 0.1);

    pc.printf("Start\n");
    motors[0].init();
    pc.attach(&serialInterrupt);
    int count = 0;
    while(1) {
        if (count % 20 == 0) {
            pc.printf("s:%d\n", motors[0].getSpeed());
        }
        if (serialData) {
            char temp[16];
            memcpy(temp, buf, 16);
            memset(buf, 0, 16);
            serialData = false;
            parseCommad(temp);
        }
        //motors[0].pid(motor0Ticks);
        //motor0Ticks = 0;
        wait_ms(50);
        count++;

        //pc.printf("buf: %s\n", buf);
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
    if (command[0] == 's' && command[1] == 'd') {
        int16_t speed = atoi(command + 2);
        motors[0].pid_on = 1;
        motors[0].setSpeed(speed);
    }
    if (command[0] == 's') {
        pc.printf("s:%d\n", motors[0].getSpeed());
    } else if (command[0] == 'w' && command[1] == 'l') {
        int16_t speed = atoi(command + 2);
        motors[0].pid_on = 0;
        if (speed < 0) motors[0].backward(-1*speed/255.0);
        else motors[0].forward(speed/255.0);
    } else if (command[0] == 'p' && command[1] == 'p') {
        uint8_t pGain = atoi(command + 2);
        motors[0].pgain = pGain;
    } else if (command[0] == 'p' && command[1] == 'i') {
        uint8_t iGain = atoi(command + 2);
        motors[0].igain = iGain;
    } else if (command[0] == 'p' && command[1] == 'd') {
        uint8_t dGain = atoi(command + 2);
        motors[0].dgain = dGain;
    } else if (command[0] == 'p') {
        char gain[20];
        motors[0].getPIDGain(gain);
        pc.printf("%s\n", gain);
    }
}

void motor0EncTick() {
    uint8_t enc_dir;
    motor0EncNow = MOTOR0_ENCA.read() | (MOTOR0_ENCB.read() << 1);
    enc_dir = (motor0EncLast & 1) ^ ((motor0EncNow & 2) >> 1);
    motor0EncLast = motor0EncNow;
    //_led = enc_dir;
    //_pc->printf("%d\n", enc_now);

    if (enc_dir & 1) motor0Ticks++;
    else motor0Ticks--;
}

void motor0PidTick() {
    motors[0].pid2(motor0Ticks);
    motor0Ticks = 0;
}
