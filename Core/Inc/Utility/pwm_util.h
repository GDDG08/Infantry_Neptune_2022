/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : pwm_util.h
 *  Description  : This file contains the PWM functions
 *  LastEditors  : ����ؼ���ᶯ��
 *  Date         : 2021-06-11 14:19:14
 *  LastEditTime : 2021-06-13 23:39:09
 */

#ifndef PWM_UTIL_H
#define PWM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"
#if __FN_IF_ENABLE(__FN_UTIL_PWM)

#include "tim.h"

typedef enum {
    PWM_OFF = 0,
    PWM_ON = 1
} PWM_PWMStateEnum;

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t ch;
    uint32_t clk;
    uint32_t period;
    TIM_OC_InitTypeDef conf;
    float duty;
    uint32_t freq;
    PWM_PWMStateEnum state;
} PWM_PWMHandleTypeDef;

void PWM_InitPWM(PWM_PWMHandleTypeDef* pwm, TIM_HandleTypeDef* htim, uint32_t ch);
void PWM_StartPWM(PWM_PWMHandleTypeDef* pwm);
void PWM_StopPWM(PWM_PWMHandleTypeDef* pwm);
void PWM_SetPWMDuty(PWM_PWMHandleTypeDef* pwm, float duty);
void PWM_SetPWMFreq(PWM_PWMHandleTypeDef* pwm, uint32_t freq);

#endif

#ifdef __cplusplus
}
#endif

#endif
