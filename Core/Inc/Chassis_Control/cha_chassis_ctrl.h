/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : cha_chassis_ctrl.h
 *  Description  : This file contains chassis control function
 *  LastEditors  : ????ؼ???ᶯ??
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 09:00:56
 */

#ifndef CHA_CHASSIS_CTRL_H
#define CHA_CHASSIS_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_CTRL_CHASSIS)

#include "motor_periph.h"
#include "pid_alg.h"
#include "math_alg.h"

typedef enum {
    Chassis_MODE_NULL = 0,
    Chassis_MODE_INIT = 1,
    Chassis_MODE_STOP = 2,
    Chassis_MODE_NORMAL = 3,
    Chassis_MODE_GYRO = 4
} Chassis_ChassisModeEnum;

typedef struct {
    float forward_back_ref;  // Forward backward translation speed
    float left_right_ref;    // Left right translation speed
    float rotate_ref;        // Rotation speed
} Chassis_ChassisRefTypeDef;

typedef struct {
    Chassis_ChassisRefTypeDef last_ref;       // Last target value
    Chassis_ChassisRefTypeDef raw_ref;        // Original headless speed target value
    Chassis_ChassisRefTypeDef raw_speed_ref;  // Original chassis speed target value
    Chassis_ChassisRefTypeDef power_ref;      // Target value after power control

    float last_yaw_ref;                // Last PTZ raw target value
    PID_PIDTypeDef Chassis_followPID;  // Chassis servo PID

    uint8_t control_state;                    // Enable control 1 yes 0 no
    uint8_t output_state;                     // Enable output 1 yes 0 no
    uint8_t pending_state;                    // Chassis occupancy lock 1 yes 0 no
    uint8_t mode_changed;                     // Does chassis mode change 1 yes 0 no
    Chassis_ChassisModeEnum mode, last_mode;  // Chassis mode, mode before chassis
    Motor_MotorParamTypeDef* current_param;   // Current chassis motor parameters
} Chassis_ChassisTypeDef;

extern Motor_MotorParamTypeDef Chassis_chassisMotorParamInit;
extern Motor_MotorParamTypeDef Chassis_chassisMotorParamStop;
extern Motor_MotorParamTypeDef Chassis_chassisMotorParamNormal;
extern Motor_MotorParamTypeDef Chassis_chassisMotorParamBrake;
extern Motor_MotorParamTypeDef Chassis_chassisMotorParamGyro;
extern Motor_MotorParamTypeDef Chassis_chassisMotorParamSlopeJump;

extern PID_PIDParamTypeDef Chassis_followPIDParam;

void Chassis_Task(void const* argument);
void Chassis_InitChassis(void);
Chassis_ChassisTypeDef* Chassis_GetChassisControlPtr(void);
void Chassis_SetChassisControlState(uint8_t state);
void Chassis_SetChassisOutputState(uint8_t state);
void Chassis_SetForwardBackRef(float ref);
void Chassis_SetLeftRightRef(float ref);
void Chassis_SetRotateRef(float ref);
void Chassis_SetMode(Chassis_ChassisModeEnum mode);
void Chassis_SetStopRef(void);
void Chassis_ClearChassisRef(Chassis_ChassisRefTypeDef* pref);
void Chassis_CopyChassisRef(Chassis_ChassisRefTypeDef* dest, Chassis_ChassisRefTypeDef* src);
void Chassis_CalcFollowRef(void);
void Chassis_CalcGyroRef(void);
void Chassis_CalcMecanumRef(void);
void Chassis_Control(void);
void Chassis_Output(void);

#ifdef __cplusplus
}
#endif

#endif

#endif
