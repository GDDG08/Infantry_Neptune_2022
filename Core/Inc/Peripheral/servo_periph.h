/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : servo_periph.h
 *  Description  : This document contains the relevant auxiliary functions of steering gear control
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-07 03:27:45
 */

#ifndef SERVO_PERIPH_H
#define SERVO_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_PERIPH_SERVO)

#include "pwm_util.h"

typedef enum {
    Servo_OFF = 0,
    Servo_ON = 1
} Servo_ServoStateEnum;

typedef struct {
    PWM_PWMHandleTypeDef pwm;
    float angle;
    Servo_ServoStateEnum state;
} Servo_ServoTypeDef;

extern Servo_ServoTypeDef Servo_ammoContainerCapServo;

void Servo_InitAllServos(void);
Servo_ServoStateEnum Servo_GetServoState(Servo_ServoTypeDef* servo);
void Servo_StartServo(Servo_ServoTypeDef* servo);
void Servo_StopServo(Servo_ServoTypeDef* servo);
float Servo_GetServoAngle(Servo_ServoTypeDef* servo);
void Servo_SetServoAngle(Servo_ServoTypeDef* servo, float angle);
void Servo_InitServo(Servo_ServoTypeDef* servo, TIM_HandleTypeDef* htim, uint32_t ch);

#endif

#ifdef __cplusplus
}
#endif

#endif
