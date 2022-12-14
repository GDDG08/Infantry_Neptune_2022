/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : gim_gimbal_ctrl.c
 *  Description  : This file contains Gimbal control function
 *  LastEditors  : ????ؼ???ᶯ??
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-25 11:55:00
 */

#include "gim_gimbal_ctrl.h"

#if __FN_IF_ENABLE(__FN_CTRL_GIMBAL_GIM)

#include "const.h"
#include "gim_remote_ctrl.h"
#include "gim_shoot_ctrl.h"
#include "gim_miniPC_ctrl.h"
#include "gim_ins_ctrl.h"
#include "buscomm_ctrl.h"
#include "cmsis_os.h"

#define GIMBAL_TASK_PERIOD 1

Motor_MotorParamTypeDef GimbalPitch_gimbalPitchMotorParamBigEnergy;
Motor_MotorParamTypeDef GimbalPitch_gimbalPitchMotorParamSmallEnergy;
Motor_MotorParamTypeDef GimbalPitch_gimbalPitchMotorParamArmor;
Motor_MotorParamTypeDef GimbalPitch_gimbalPitchMotorParamIMUDebug;
Motor_MotorParamTypeDef GimbalPitch_gimbalPitchMotorParamNoAuto;

Gimbal_GimbalTypeDef Gimbal_gambalControlData;

/**
  * @brief          Gimbal task
  * @param          NULL
  * @retval         NULL
  */
void Gimbal_Task(void const* argument) {
    for (;;) {
        while (!GLOBAL_INIT_FLAG) {
            osDelay(1);
        }

        MiniPC_CalcAutoAim();
        Gimbal_CtrlPitch();
        Gimbal_CtrlYaw();
        GimbalPitch_Output();
        osDelay(GIMBAL_TASK_PERIOD);
    }
}

/**
* @brief      Gimbal initialization offset and mode
* @param      NULL
* @retval     NULL
*/
void Gimbal_InitOffset() {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    gimbal->output_state = 1;
    gimbal->control_state = 1;

    gimbal->mode.mode_change_flag = 0;
    gimbal->mode.present_mode = Gimbal_IMU_DEBUG;
    gimbal->mode.last_mode = Gimbal_IMU_DEBUG;

    gimbal->yaw_mode = GimbalYaw_MODE_IMU_DEBUG;

    Motor_gimbalMotorPitch.encoder.angle = 4000;

    Const_SetGimbalPitchMotorParam();

    HAL_Delay(1000);

    Gimbal_ChangeMode(Gimbal_NOAUTO);
}

/**
  * @brief      Gets the pointer to the gimbal control object
  * @param      NULL
  * @retval     The pointer points to the gimbal control object
  */
Gimbal_GimbalTypeDef* Gimbal_GetGimbalControlPtr() {
    return &Gimbal_gambalControlData;
}

/**
* @brief      Change Gimbal mode
* @param      mode: Gimbal mode enum
* @retval     NULL
*/
void Gimbal_ChangeMode(Gimbal_ModeEnum mode) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();
    gimbal->mode.last_mode = gimbal->mode.present_mode;
    gimbal->mode.present_mode = mode;

    if (gimbal->mode.last_mode != gimbal->mode.present_mode)
        gimbal->mode.mode_change_flag = 1;
}

/**
* @brief      Yaw state control
* @param      NULL
* @retval     NULL
*/
void Gimbal_CtrlYaw() {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    if (gimbal->control_state == 0)
        return;

    switch (gimbal->mode.present_mode) {
        case Gimbal_NOAUTO:
            gimbal->yaw_mode = GimbalYaw_MODE_NO_AUTO;
            break;
        case Gimbal_ARMOR:
            gimbal->yaw_mode = GimbalYaw_MODE_ARMOR;
            break;
        case Gimbal_IMU_DEBUG:
            gimbal->yaw_mode = GimbalYaw_MODE_IMU_DEBUG;
            break;
        case Gimbal_BIG_ENERGY:
            gimbal->yaw_mode = GimbalYaw_MODE_BIG_ENERGY;
            break;
        case Gimbal_SMALL_ENERGY:
            gimbal->yaw_mode = GimbalYaw_MODE_SMALL_ENERGY;
            break;
    }
}

/**
* @brief      Pitch state control
* @param      NULL
* @retval     NULL
*/
void Gimbal_CtrlPitch() {
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();

    // Set pid param
    Motor_MotorParamTypeDef* pparam;
    if (gimbal->control_state == 0)
        return;

    // clear pid param before changing mode
    if (gimbal->mode.mode_change_flag == 1) {
        Motor_ResetMotorPID(&Motor_gimbalMotorPitch);
        gimbal->mode.mode_change_flag = 0;
    }
    switch (gimbal->mode.present_mode) {
        case Gimbal_NOAUTO:
            pparam = &GimbalPitch_gimbalPitchMotorParamNoAuto;
            break;
        case Gimbal_ARMOR:
            pparam = &GimbalPitch_gimbalPitchMotorParamArmor;
            break;
        case Gimbal_IMU_DEBUG:
            pparam = &GimbalPitch_gimbalPitchMotorParamIMUDebug;
            break;
        case Gimbal_BIG_ENERGY:
            pparam = &GimbalPitch_gimbalPitchMotorParamBigEnergy;
            break;
        case Gimbal_SMALL_ENERGY:
            pparam = &GimbalPitch_gimbalPitchMotorParamSmallEnergy;
            break;
        default:
            break;
    }

    if (gimbal->mode.present_mode == Gimbal_IMU_DEBUG) {
        gimbal->pitch_position_fdb = (Motor_gimbalMotorPitch.encoder.limited_angle - Const_PITCH_MOTOR_INIT_OFFSET);
        gimbal->pitch_speed_fdb = Motor_gimbalMotorPitch.encoder.speed;
    } else {
        gimbal->pitch_position_fdb = imu->angle.pitch;
        gimbal->pitch_speed_fdb = imu->speed.pitch;
    }

    Motor_SetMotorRef(&Motor_gimbalMotorPitch, gimbal->angle.pitch_angle_ref);

    Motor_SetMotorFdb(&Motor_gimbalMotorPitch, 2, gimbal->pitch_position_fdb);
    Motor_SetMotorFdb(&Motor_gimbalMotorPitch, 1, gimbal->pitch_speed_fdb);
    Motor_CalcMotorOutput(&Motor_gimbalMotorPitch, pparam);
}

/**
* @brief      Pitch angle limit
* @param      ref: Pitch set ref
* @retval     Limited pitch ref
*/
float Gimbal_LimitPitch(float ref) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    float pitch_umaxangle;
    if (buscomm->chassis_mode == CHASSIS_CTRL_GYRO) {
        pitch_umaxangle = Const_PITCH_UMAXANGLE_GRYO;
    } else {
        pitch_umaxangle = Const_PITCH_UMAXANGLE;
    }

    if (((gimbal->angle.pitch_angle_ref > pitch_umaxangle) && (ref > 0)) ||
        ((gimbal->angle.pitch_angle_ref < Const_PITCH_DMAXANGLE) && (ref < 0)))
        return 0.0f;
    // Out of depression set maximum ref
    else
        return ref;
}

/**
* @brief      Yaw angle limit
* @param      ref: Yaw set ref
* @retval     Limited ywa ref
*/
float Gimbal_LimitYaw(float ref) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    if (buscomm->chassis_mode == CHASSIS_CTRL_GYRO)
        return ref;
    else if (((buscomm->yaw_relative_angle < -Const_YAW_MAXANGLE) && (ref > 0)) ||
             ((buscomm->yaw_relative_angle > Const_YAW_MAXANGLE) && (ref < 0)))
        return 0.0f;
    else
        return ref;
}

/**
* @brief      Set pitch ref
* @param      ref: Yaw set ref
* @retval     NULL
*/
void Gimbal_SetPitchRef(float ref) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    gimbal->angle.pitch_angle_ref += ref;
}

/**
* @brief      Aoto aim mode set pitch ref
* @param      ref: Yaw set ref
* @retval     NULL
*/
void Gimbal_SetPitchAutoRef(float ref) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();

    float limited_ref;
    if (ref > Const_PITCH_UMAXANGLE)
        limited_ref = Const_PITCH_UMAXANGLE;
    else if (ref < Const_PITCH_DMAXANGLE)
        limited_ref = Const_PITCH_DMAXANGLE;
    else
        limited_ref = ref;
    // Out of depression set maximum ref
    gimbal->angle.pitch_angle_ref = limited_ref;  //imu->angle.pitch + ref
}

/**
* @brief      Set yaw ref
* @param      ref: Pitch set ref
* @retval     NULL
*/
void Gimbal_SetYawRef(float ref) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    gimbal->angle.yaw_angle_ref -= ref;
}

/**
* @brief      Aoto aim mode set yaw ref
* @param      ref: Yaw set ref
* @retval     NULL
*/
void Gimbal_SetYawAutoRef(float ref) {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();

    gimbal->angle.yaw_angle_ref = Gimbal_LimitYaw(ref);  //imu->angle.yaw - ref
}

/**
  * @brief      Gimbal pitch output function
  * @param      NULL
  * @retval     NULL
  */
void GimbalPitch_Output() {
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    if (gimbal->output_state == 1) {
        Motor_SendMotorGroupOutput(&Motor_gimbalMotors);
    }
}

#endif
