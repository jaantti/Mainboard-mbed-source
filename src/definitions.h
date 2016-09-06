#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_ value

#define NUMBER_OF_MOTORS 1

#define MOTOR_PWM(n) MOTOR ## n ## _PWM
#define MOTOR_DIR1(n) MOTOR ## n ## _DIR1
#define MOTOR_DIR2(n) MOTOR ## n ## _DIR2
#define MOTOR_FAULT(n) MOTOR ## n ## _FAULT
#define MOTOR_ENCA(n) MOTOR ## n ## _ENCA
#define MOTOR_ENCB(n) MOTOR ## n ## _ENCB

#define MOTOR_ENC_TICK(i)   void motor ## i ## EncTick() { \
                                uint8_t enc_dir; \
                                motorEncNow[i] = MotorEncA[i]->read() | (MotorEncB[i]->read() << 1); \
                                enc_dir = (motorEncLast[i] & 1) ^ ((motorEncNow[i] & 2) >> 1); \
                                motorEncLast[i] = motorEncNow[i]; \
                                \
                                if (enc_dir & 1) motorTicks[i]++; \
                                else motorTicks[i]--; \
                            }

#define MOTOR_PID_TICK(i)   void motor ## i ## PidTick() { \
                                const uint8_t motor = 0; \
                                motors[motor].pid2(motorTicks[motor]); \
                                motorTicks[motor] = 0; \
                            }
#endif
