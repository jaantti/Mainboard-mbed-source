#ifndef MOTOR_H
#define  MOTOR_H
#include "mbed.h"
#include "PwmOut.h"
#include "USBSerial.h"

class Motor {
public:
    Motor() = default;
    Motor(USBSerial *pc, PwmOut *pwm, DigitalOut *dir1, DigitalOut *dir2, DigitalIn *fault);
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

    USBSerial *_pc;
    PwmOut *_pwm;
    DigitalOut *_dir1;
    DigitalOut *_dir2;
    DigitalIn *_fault;

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

    float pidSpeed0;
    float pidSpeed1;
    float pidError0;
    float pidError1;
    float pidError2;
    float pidSetpoint;

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
