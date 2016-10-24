#include "motor.h"

Motor::Motor(USBSerial *pc, PwmOut *pwm, DigitalOut *dir1, DigitalOut *dir2, DigitalIn *fault) {
    _pc = pc;
    _pwm = pwm;
    _dir1 = dir1;
    _dir2 = dir2;
    _fault = fault;

    enc_last = 0;

    pid_on = 1;
    currentPWM = 0;

    encTicks = 0;
    motor_polarity = 0;

    pidSpeed0 = 0;
    pidSpeed1 = 0;
    pidError0 = 0;
    pidError1 = 0;
    pidError2 = 0;
    pidSetpoint = 0;

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

void Motor::pid2(int16_t encTicks) {
    speed = encTicks;
    float pidDiv = 100.0;

    if (!pid_on) return;
    pidError2 = pidError1;
    pidError1 = pidError0;
    pidError0 = pidSetpoint - speed;
    pidSpeed1 = pidSpeed0;
    float p = pgain / pidDiv * (pidError0 - pidError1);
    float i = igain / pidDiv * (pidError0 + pidError1) / 2.0;
    float d = dgain / pidDiv * (pidError0 - 2.0 * pidError1 + pidError2);
    pidSpeed0 = pidSpeed1 + p + i + d;


    if (pidSpeed0 > 255) pidSpeed0 = 255;
    if (pidSpeed0 < -255) pidSpeed0 = -255;

    if (pidSpeed0 > 0) forward((pidSpeed0) / 255.0);
    else backward((pidSpeed0) / -255.0);

    if (pidSetpoint == 0) {
        forward(0);
        pidError0 = 0;
        pidError1 = 0;
        pidError2 = 0;
        pidSpeed0 = 0;
        pidSpeed1 = 0;
    }
}

void Motor::pid(int16_t encTicks) {
    speed = encTicks;
    encTicks = 0;

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

        _pc->printf("pwm:%d\n", pwm);

        if (pwm < 0) {
            pwm *= -1;
            backward(pwm/255.0);
        } else {
            forward(pwm/255.0);
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


void Motor::setup() {
    pid_on = 0;
    forward(50);
    encTicks = 0;
    wait_ms(500);

    if (speed < 0) {
        motor_polarity ^= 1;
    }

    reset_pid();
    forward(0);
    pid_on = 1;
}

void Motor::init() {
    dir = 0;
    motor_polarity = 0;
    pgain = 50;
    igain = 10;
    dgain = 0;
    pid_multi = 32;
    imax = 255*pid_multi;
    err_max = 4000;
    pwm_min = 25;
    intgrl = 0;
    //count = 0;
    speed = 0;
    err = 0;
}

int16_t Motor::getSpeed() {
    return speed;
}

void Motor::setSpeed(int16_t speed) {
    sp_pid = speed;
    pidSetpoint = speed;
    if (sp_pid == 0) reset_pid();
    fail_counter = 0;
}

void Motor::getPIDGain(char *gain) {
    sprintf(gain, "PID:%d,%d,%d", pgain, igain, dgain);
}
