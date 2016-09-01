#include "motor.h"

Motor::Motor(Serial *pc, PwmOut *pwm, DigitalOut *dir1, DigitalOut *dir2, DigitalIn *fault, InterruptIn *enca, InterruptIn *encb, DigitalOut *led) {
    _pc = pc;
    _pwm = pwm;
    _dir1 = dir1;
    _dir2 = dir2;
    _fault = fault;
    _enca = enca;
    _encb = encb;
    _led = led;

    enc_last = 0;

    _enca->rise(this, &Motor::decode);
    _enca->fall(this, &Motor::decode);
    _encb->rise(this, &Motor::decode);
    _encb->fall(this, &Motor::decode);

    pid_on = 1;
    motor_polarity = 1;
    fail_counter = 0;
    send_speed = 0;
    failsafe = 1;
    leds_on = 1;

    stall_counter = 0;
    stallCount = 0;
    prevStallCount = 0;
    stallWarningLimit = 60;
    stallErrorLimit = 300;
    stallLevel = 0;
    stallChanged = 0;
    currentPWM = 0;

    wcount.value = 0;

    _pwm->period_us(PWM_PERIOD_US);
}

void Motor::forward(float pwm) {
    if (dir) {
        *_dir1 = 0;
        *_dir2 = 1;
    } else {
        *_dir1 = 1;
        *_dir2 = 0;
    }

    *_pwm = pwm;
    currentPWM = pwm;
}

void Motor::backward(float pwm) {
    if (dir) {
        *_dir1 = 1;
        *_dir2 = 0;
    } else {
        *_dir1 = 0;
        *_dir2 = 1;
    }

    *_pwm = pwm;
    currentPWM = -pwm;
}

void Motor::pid() {
    decoder_count.value = wcount.value;
    speed = wcount.value;
    wcount.value = 0;

    if (!pid_on) return;

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

    if ((fail_counter == 100) && failsafe) {
        sp_pid = 0;
        reset_pid();
        forward(0);
    }

    if (stallChanged) {
        _pc->printf("<stall:%d>\n", stallLevel);
        stallChanged = 0;
    }

    if ((speed < 10) && (pwm > 250)) {
        stall_counter++;
    } else {
        stall_counter = 0;
    }

    fail_counter++;

    if ((fail_counter == 100) && failsafe) {
        sp_pid = 0;
        reset_pid();
        forward(0);
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
    enc_now = _enca->read() | (_encb->read() << 1);
    enc_dir = (enc_last & 1) ^ ((enc_now & 2) >> 1);
    enc_last = enc_now;
    *_led = enc_dir;
    //_pc->printf("%d\n", enc_now);
    if (enc_dir & 1) {
        if (motor_polarity) wcount.value--;
        else wcount.value++;
    }
    else {
        if (motor_polarity) wcount.value++;
        else wcount.value--;
    }
    //_pc->printf("wcount:%d\n", wcount.value);
}

void Motor::setup() {
    pid_on = 0;
    forward(50);
    wcount.value = 0;
    wait_ms(500);

    if (speed < 0) {
        motor_polarity ^= 1;
    }

    sp_pid = 0;
    reset_pid();
    forward(0);
    pid_on = 1;
}

void Motor::init() {
    dir = 0;
    motor_polarity = 0;
    pgain = 6;
    igain = 8;
    pid_multi = 32;
    imax = 255*pid_multi;
    err_max = 4000;
    pwm_min = 25;
    intgrl = 0;
    count = 0;
    speed = 0;
    err = 0;
}

int16_t Motor::getSpeed() {
    return speed;
}

void Motor::setSpeed(int16_t speed) {
    sp_pid = speed;
    if (sp_pid == 0) reset_pid();
    fail_counter = 0;
}

void Motor::getPIDGain(char *gain) {
    sprintf(gain, "PID:%d,%d,%d", pgain, igain, dgain);
}
