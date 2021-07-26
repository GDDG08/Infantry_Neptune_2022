/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : supercap_ctrl.h
 *  Description  : This file contains cap control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 08:49:33
 */

#ifndef SUPERCAP_CTRL_H
#define SUPERCAP_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_SUPER_CAP)

#include "sensor_periph.h"
#include "led_periph.h"
#include "dac_util.h"
#include "gpio_util.h"

typedef enum {
    Power_PATH_NULL = 0,
    Power_PATH_REFEREE = 1,
    Power_PATH_CAP = 2
} POWER_PathEnum;

typedef enum {
    CAP_MODE_OFF = 0,  //Super cap off
    CAP_MODE_ON = 1,   //Super cap is on
    CAP_MODE_ERROR = 2
} CAP_StateEnum;

typedef enum {
    CAP_CHARGE_OFF = 0,
    CAP_CHARGE_ON = 1
} CAP_ChargeStateEnum;

typedef struct {
    float power_limit;

    CAP_ChargeStateEnum charge_state;
    POWER_PathEnum power_path;
    CAP_StateEnum cap_state;
} CAP_ControlValueTypeDef;

extern CAP_ControlValueTypeDef Cap_ControlState;

void SuperCap_Task(void const* argument);
void Cap_SetChargeCurrent(float current);
void Cap_Init(void);
CAP_ControlValueTypeDef* Cap_GetCapControlPtr(void);
void Cap_JudgeCapState(void);
void Cap_CapCharge(void);
void Cap_ChangePowerPath(POWER_PathEnum path);
void Cap_Control(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
