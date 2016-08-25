#ifndef MOTOR_H
#define  MOTOR_H
#include "mbed.h"
#include "PwmOut.h"

class Motor {
public:
  Motor(PinName pwm, PinName dir1, PinName dir2, PinName fault, PinName enca, PinName encb);
  void setup();
private:
  const uint PWM_PERIOD_US = 100;

  PwmOut *_pwm;
  DigitalOut *_dir1;
  DigitalOut *_dir2;
  DigitalIn *_fault;
  InterruptIn *_enca;
  InterruptIn *_encb;

  union doublebyte {
    unsigned int value;
    unsigned char bytes[2];
  };

  uint8_t enc_dir;
  uint8_t enc_last;
  uint8_t enc_now;
  union doublebyte wcount;
  union doublebyte decoder_count;

  uint8_t dir;
  uint8_t pid_on = 1;
  uint8_t motor_polarity = 1;
  uint8_t fail_counter = 0;
  uint8_t send_speed = 0;
  uint8_t failsafe = 1;
  uint8_t leds_on = 1;

  int16_t sp, sp_pid, der;
  int16_t intgrl, prop;
  int16_t count, speed;
  int16_t csum;
  int16_t err, err_prev;
  uint8_t pgain, igain, dgain;
  int16_t pwm, pwm_min, pwmmin;
  int16_t imax, err_max;
  uint8_t pid_multi, update_pid;
  uint8_t ball = 0;
  uint8_t ir_led;
  uint16_t stall_counter = 0;
  uint16_t stallCount = 0;
  uint16_t prevStallCount = 0;
  uint16_t stallWarningLimit = 60;
  uint16_t stallErrorLimit = 300;
  uint8_t stallLevel = 0;
  uint8_t stallChanged = 0;
  int16_t currentPWM = 0;

  void forward(float pwm);
  void backward(float pwm);
  void pid();
  void reset_pid();
  void decode();
};
#endif //MOTOR_H
