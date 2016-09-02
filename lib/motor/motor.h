#ifndef MOTOR_H
#define  MOTOR_H
#include "mbed.h"
#include "PwmOut.h"

class Motor {
public:
    Motor() = default;
    Motor(Serial *pc, PwmOut *pwm, DigitalOut *dir1, DigitalOut *dir2, DigitalIn *fault, InterruptIn *enca, InterruptIn *encb, DigitalOut *led);
    Motor (const Motor& ) = default;

    uint8_t pid_on;
    int16_t encTicks;
    volatile int16_t speed;
    uint8_t pgain, igain, dgain;

    void setup();
    /*
     * Run this to update speed
     */
    void pid(int16_t encTicks);
    void pid2(int16_t encTicks);
    void init();
    int16_t getSpeed();
    void setSpeed (int16_t speed);
    void getPIDGain(char *gain);
    void forward(float pwm);
    void backward(float pwm);
private:
    static const uint PWM_PERIOD_US = 1000;

    Serial *_pc;
    PwmOut *_pwm;
    DigitalOut *_dir1;
    DigitalOut *_dir2;
    DigitalIn *_fault;
    InterruptIn *_enca;
    InterruptIn *_encb;
    DigitalOut *_led;

    union doublebyte {
        unsigned int value;
        unsigned char bytes[2];
    };

    uint8_t enc_dir;
    uint8_t enc_last;
    uint8_t enc_now;

    uint8_t dir;

    int16_t currentPWM;
    uint8_t motor_polarity;

    int16_t pidSpeed0;
    int16_t pidSpeed1;
    int16_t pidError0;
    int16_t pidError1;
    int16_t pidError2;
    int16_t pidSetpoint;

/*
    union doublebyte wcount;
    union doublebyte decoder_count;

    uint8_t dir;
*/

    uint8_t fail_counter;
    uint8_t send_speed;
    uint8_t failsafe;
    uint8_t leds_on;

    int16_t sp, sp_pid, der;
    int16_t intgrl, prop;
    //int16_t count, speed;
    int16_t csum;
    int16_t err, err_prev;

    int16_t pwm, pwm_min, pwmmin;
    int16_t imax, err_max;
    uint8_t pid_multi, update_pid;

    uint16_t stall_counter;
    uint16_t stallCount;
    uint16_t prevStallCount;
    uint16_t stallWarningLimit;
    uint16_t stallErrorLimit;
    uint8_t stallLevel;
    uint8_t stallChanged;
    //int16_t currentPWM;


    void reset_pid();

};
#endif //MOTOR_H
