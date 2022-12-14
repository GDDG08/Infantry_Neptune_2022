/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : cha_chassis_ctrl.c
 *  Description  : This file contains chassis control function
 *  LastEditors  : ????ؼ???ᶯ??
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 09:00:35
 */

#include "cha_chassis_ctrl.h"

#if __FN_IF_ENABLE(__FN_CTRL_CHASSIS)

#include "cha_gimbal_ctrl.h"
#include "cha_power_ctrl.h"
#include "cmsis_os.h"

#include "const.h"

#define CHASSIS_TASK_PERIOD 1
Motor_MotorParamTypeDef Chassis_chassisMotorParamInit;
Motor_MotorParamTypeDef Chassis_chassisMotorParamStop;
Motor_MotorParamTypeDef Chassis_chassisMotorParamNormal;
Motor_MotorParamTypeDef Chassis_chassisMotorParamGyro;

PID_PIDParamTypeDef Chassis_followPIDParam;
Chassis_ChassisTypeDef Chassis_chassisControlData;

/**
  * @brief          Chassis task
  * @param          NULL
  * @retval         NULL
  */
void Chassis_Task(void const* argument) {
    for (;;) {
        while (!GLOBAL_INIT_FLAG) {
            osDelay(1);
        }
        Chassis_Control();
        Chassis_Output();
        osDelay(CHASSIS_TASK_PERIOD);
    }
}

/**
  * @brief      Chassis control initialization
  * @param      NULL
  * @retval     NULL
  */
void Chassis_InitChassis() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();

    chassis->pending_state = 1;

    chassis->control_state = 1;
    chassis->output_state = 1;

    chassis->mode = Chassis_MODE_STOP;
    chassis->last_mode = Chassis_MODE_STOP;
    chassis->mode_changed = 0;

    Chassis_SetStopRef();

    // Initialization of motor parameters (including PID parameters)
    Const_SetChasisMotorParam();

    chassis->pending_state = 0;
}

/**
  * @brief      Gets the pointer to the chassis control object
  * @param      NULL
  * @retval     The pointer points to the chassis control object
  */
Chassis_ChassisTypeDef* Chassis_GetChassisControlPtr() {
    return &Chassis_chassisControlData;
}

/**
  * @brief      Set the chassis control output calculation enable state
  * @param      state: Enabled, 1 is enabled, 0 is disabled
  * @retval     NULL
  */
void Chassis_SetChassisControlState(uint8_t state) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    chassis->control_state = state;
}

/**
  * @brief      Set chassis control output enable status
  * @param      state: Enabled, 1 is enabled, 0 is disabled
  * @retval     NULL
  */
void Chassis_SetChassisOutputState(uint8_t state) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    chassis->output_state = state;
}

/**
  * @brief      Chassis front and rear motion control setting
  * @param      ref: Front and rear motion control of chassis
  * @retval     NULL
  */
void Chassis_SetForwardBackRef(float ref) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    chassis->last_ref.forward_back_ref = chassis->raw_ref.forward_back_ref;
    chassis->raw_ref.forward_back_ref = ref;
}

/**
  * @brief      Chassis left and right motion control setting
  * @param      ref: Chassis left and right motion control
  * @retval     NULL
  */
void Chassis_SetLeftRightRef(float ref) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    chassis->last_ref.left_right_ref = chassis->raw_ref.left_right_ref;
    chassis->raw_ref.left_right_ref = ref;
}

/**
  * @brief      Control quantity setting of chassis rotary motion
  * @param      ref: Control quantity of chassis rotary motion
  * @retval     NULL
  */
void Chassis_SetRotateRef(float ref) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    chassis->last_ref.rotate_ref = chassis->raw_ref.rotate_ref;
    chassis->raw_ref.rotate_ref = ref;
}

/**
  * @brief      Chassis mode setting
  * @param      mode: Chassis mode
  * @retval     NULL
  */
void Chassis_SetMode(Chassis_ChassisModeEnum mode) {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();

    chassis->last_mode = chassis->mode;
    chassis->mode = mode;
    if (chassis->last_mode != chassis->mode)
        chassis->mode_changed = 1;
    else
        chassis->mode_changed = 0;
}

/**
  * @brief      Set stop target value
  * @param      NULL
  * @retval     NULL
  */
void Chassis_SetStopRef() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();

    Chassis_SetForwardBackRef(0);
    Chassis_SetLeftRightRef(0);
    Chassis_SetRotateRef(0);
    Chassis_ClearChassisRef(&(chassis->raw_speed_ref));
}

/**
  * @brief      Set stop target value
  * @param      NULL
  * @retval     NULL
  */
void Chassis_CalcMoveRef() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();

    float theta_rad;
    if (chassis->mode == Chassis_MODE_GYRO) {
        theta_rad = -(Motor_gimbalMotorYaw.encoder.limited_angle - Const_YAW_MOTOR_INIT_OFFSET) * PI / 180 - 0.5f;
    } else {
        theta_rad = -(Motor_gimbalMotorYaw.encoder.limited_angle - Const_YAW_MOTOR_INIT_OFFSET) * PI / 180;
    }

    float sin_tl = (float)sin(theta_rad);
    float cos_tl = (float)cos(theta_rad);
    chassis->raw_speed_ref.forward_back_ref = chassis->raw_ref.forward_back_ref * cos_tl - chassis->raw_ref.left_right_ref * sin_tl;
    chassis->raw_speed_ref.left_right_ref = chassis->raw_ref.forward_back_ref * sin_tl + chassis->raw_ref.left_right_ref * cos_tl;
}

/**
  * @brief      Chassis following solution
  * @param      NULL
  * @retval     NULL
  */
void Chassis_CalcFollowRef() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    GimbalYaw_GimbalYawTypeDef* gimbalyaw = GimbalYaw_GetGimbalYawPtr();

    chassis->raw_speed_ref.rotate_ref = chassis->raw_ref.rotate_ref;
    PID_SetPIDRef(&(chassis->Chassis_followPID), 0);
    PID_SetPIDFdb(&(chassis->Chassis_followPID), Motor_gimbalMotorYaw.encoder.limited_angle - Const_YAW_MOTOR_INIT_OFFSET);
    if ((fabs(chassis->Chassis_followPID.ref - chassis->Chassis_followPID.fdb) < 3) && (gimbalyaw->yaw_ref == chassis->last_yaw_ref)) {
        chassis->raw_speed_ref.rotate_ref += 0;
        return;
    }
    PID_CalcPID(&(chassis->Chassis_followPID), &Chassis_followPIDParam);
    chassis->raw_speed_ref.rotate_ref += PID_GetPIDOutput(&(chassis->Chassis_followPID));
    chassis->last_yaw_ref = gimbalyaw->yaw_ref;
}

/**
  * @brief      Calculation of chassis small gyroscope
  * @param      NULL
  * @retval     NULL
  */
inline float sqr(float x) {
    return x * x;
}
void Chassis_CalcGyroRef() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    Referee_RefereeDataTypeDef* referee = Referee_GetRefereeDataPtr();

    float speed_ref = (float)sqrt(sqr(chassis->raw_speed_ref.forward_back_ref) + sqr(chassis->raw_speed_ref.left_right_ref));
    float min_vro, power_exp;
    if (referee->max_chassis_power <= 50) {
        min_vro = 480.0f;
        power_exp = 250000.0f;
    } else if (referee->max_chassis_power <= 80) {
        min_vro = 580.0f;
        power_exp = 360000.0f;
    } else if (referee->max_chassis_power <= 120) {
        min_vro = 720.0f;
        power_exp = 540000.0f;
    }
    chassis->raw_speed_ref.rotate_ref = (float)sqrt(power_exp - sqr(speed_ref));
    if (chassis->raw_speed_ref.rotate_ref < min_vro)
        chassis->raw_speed_ref.rotate_ref = min_vro;
}

/**
  * @brief      Mcnamm round solution
  * @param      NULL
  * @retval     NULL
  */
void Chassis_CalcMecanumRef() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();

    Motor_SetMotorRef(&Motor_chassisMotor1,
                      chassis->power_ref.forward_back_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.left_right_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.rotate_ref * Const_Chassis_ROTATE_REF_TO_MOTOR_REF);
    Motor_SetMotorRef(&Motor_chassisMotor2,
                      -chassis->power_ref.forward_back_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.left_right_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.rotate_ref * Const_Chassis_ROTATE_REF_TO_MOTOR_REF);
    Motor_SetMotorRef(&Motor_chassisMotor3,
                      -chassis->power_ref.forward_back_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF - chassis->power_ref.left_right_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.rotate_ref * Const_Chassis_ROTATE_REF_TO_MOTOR_REF);
    Motor_SetMotorRef(&Motor_chassisMotor4,
                      chassis->power_ref.forward_back_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF - chassis->power_ref.left_right_ref * Const_Chassis_MOVE_REF_TO_MOTOR_REF + chassis->power_ref.rotate_ref * Const_Chassis_ROTATE_REF_TO_MOTOR_REF);
}

/**
  * @brief      Copy chassis motion speed object
  * @param      dest: The pointer points to the target chassis movement speed object
  * @param      src: The pointer points to the source chassis motion speed object
  * @retval     NULL
  */
void Chassis_CopyChassisRef(Chassis_ChassisRefTypeDef* dest, Chassis_ChassisRefTypeDef* src) {
    dest->forward_back_ref = src->forward_back_ref;
    dest->left_right_ref = src->left_right_ref;
    dest->rotate_ref = src->rotate_ref;
}

/**
  * @brief      Chassis movement speed object clear
  * @param      pref: The pointer points to the chassis movement speed object
  * @retval     NULL
  */
void Chassis_ClearChassisRef(Chassis_ChassisRefTypeDef* pref) {
    pref->forward_back_ref = 0;
    pref->left_right_ref = 0;
    pref->rotate_ref = 0;
}

/**
  * @brief      Calculation of chassis control quantity
  * @param      NULL
  * @retval     NULL
  */
void Chassis_Control() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    if (chassis->control_state != 1)
        return;

    if (chassis->mode_changed == 1) {
        Motor_ResetMotorGroupPID(&Motor_chassisMotors);
        chassis->mode_changed = 0;
    }

    switch (chassis->mode) {
        case Chassis_MODE_STOP:
            chassis->current_param = &Chassis_chassisMotorParamStop;
            Chassis_SetStopRef();  // Set stop state
            break;
        case Chassis_MODE_NORMAL:
            chassis->current_param = &Chassis_chassisMotorParamNormal;
            Chassis_CalcMoveRef();    // Translation solution without head
            Chassis_CalcFollowRef();  // Chassis following solution
            break;
        case Chassis_MODE_GYRO:
            chassis->current_param = &Chassis_chassisMotorParamGyro;
            Chassis_CalcMoveRef();  // Headless translation solution
            Chassis_CalcGyroRef();  // Solution of small gyroscope
            break;
        default:
            return;
    }

    Chassis_CopyChassisRef(&(chassis->power_ref), &(chassis->raw_speed_ref));
    // Mcnamm round solution

    Chassis_CalcMecanumRef();
    // Calculation of control quantity

    Motor_CalcMotorGroupOutput(&Motor_chassisMotors, chassis->current_param);
    // Power control

    Power_PowerControl(&Motor_chassisMotors);
}

/**
  * @brief      Output chassis control quantity
  * @param      NULL
  * @retval     NULL
  */
void Chassis_Output() {
    Chassis_ChassisTypeDef* chassis = Chassis_GetChassisControlPtr();
    if (chassis->output_state != 1)
        return;

    Motor_SendMotorGroupOutput(&Motor_chassisMotors);
}

#endif
