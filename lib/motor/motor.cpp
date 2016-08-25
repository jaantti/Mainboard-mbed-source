#include "motor.h"

Motor::Motor(PinName pwm, PinName dir1, PinName dir2, PinName fault, PinName enca, PinName encb) {
  _pwm = PwmOut(pwm);
  _dir1 = DigitalOut(dir1);
  _dir2 = dir2;
  _fault = fault;
  _enca = enca;
  _encb = encb;

  enc_last = 0;

  _enca.rise(&decode);
  _encb.rise(&decode);
}

void Motor::forward(float pwm) {
  if (dir) {
    _dir1 = 0;
    _dir2 = 1;
  } else {
    _dir1 = 1;
    _dir2 = 0;
  }

  _pwm = pwm;
  currentPWM = pwm;
}

void Motor::backward(float pwm) {
  if (dir) {
    _dir1 = 1;
    _dir2 = 0;
  } else {
    _dir1 = 0;
    _dir2 = 1;
  }

  _pwm = pwm;
  currentPWM = -pwm;
}

void Motor::pid() {
  err_prev = err;
  err = sp_pid - speed;

  if (stallLevel != 2) {
    intgrl += (err * pid_multi) / igain;

    //constrain integral
    if (intgrl < -imax) intgrl = -imax;
    if (intgrl > imax) intgrl = imax;

    if (sp == 0) pwmmin = 0;
    else if (sp < 0) pwmmin = -pwm_min;
    else pwmmin = pwm_min;

    pwm = pwmmin + err*pgain + intgrl/pid_multi;
    //constrain pwm
    if (pwm < -255) pwm = -255;
    if (pwm > 255) pwm = 255;

    prevStallCount = stallCount;
    if ((speed < 5 && currentPWM == 255 || speed > -5 && currentPWM == -255) && stallCount < stallErrorLimit) {
      stallCount++;
    } else if (stallCount > 0) {
      stallCount--;
    }

    if (pwm < 0) {
      pwm *= -1;
      backward(pwm);
    } else {
      forward(pwm);
    }

    if ((stallCount == stallWarningLimit - 1) && (prevStallCount == stallWarningLimit)) {
      stallLevel = 0;
      stallChanged = 1;
    } else if ((stallCount == stallWarningLimit) && (prevStallCount == stallWarningLimit - 1)) {
      stallLevel = 1;
      stallChanged = 1;
    } else if (stallCount == stallErrorLimit) {
      stallLevel = 2;
      stallChanged = 1;
      reset_pid();
    }
  } else {
    stallCount--;
    if (stallCount == 0) {
      stallLevel = 0;
      stallChanged = 1;
    }
  }
}

void Motor::reset_pid() {
  err = 0;
  err_prev = 0;
  intgrl = 0;
  der = 0;
  sp = 0;
  sp_pid = 0;
  forward(0);
}

void Motor::decode() {
  enc_now = bit_get(PIND, 3);
  enc_dir = (enc_last & 1) ^ ((enc_now & 2) >> 1);
  enc_last = enc_now;
  if (enc_dir) {
    if (motor_polarity) wcount.value--;
    else wcount.value++;
    if (leds_on) bit_set(PORTC, BIT(LED2));
  }
  else {
    if (motor_polarity) wcount.value++;
    else wcount.value--;
    if (leds_on) bit_clear(PORTC, BIT(LED2));
  }
  if ((wcount.value == 0) && leds_on) bit_flip(PORTC, BIT(LED1));
}

void Motor::setup() {
  pid_on = 0;
  forward(50);
  wcount.value = 0;
  _delay_ms(500);
  if (speed < 0) {
    motor_polarity ^= 1;
    eeprom_update_byte((uint8_t*)1, motor_polarity);
  }
  //else motor_polarity = dir;
  sp_pid = 0;
  reset_pid();
  forward(0);
  pid_on = 1;
}
