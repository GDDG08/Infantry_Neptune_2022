/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : gim_miniPC_ctrl.c
 *  Description  : This file contains MiniPC control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-26 21:45:41
 */

#include "gim_miniPC_ctrl.h"

#if __FN_IF_ENABLE(__FN_CTRL_MINIPC)

#include "buscomm_ctrl.h"
#include "const.h"
#include "gim_gimbal_ctrl.h"
#include "gim_ins_ctrl.h"
#include "math_alg.h"
#include "key_periph.h"
#include "cmsis_os.h"

#define MINI_PC_TASK_PERIOD 1
#define dt 0.001f
MiniPC_MiniPCControlTypeDef MiniPC_MiniPCContrlData;

int CVKF_NT_PITCH = 7;

float autoaim_pitch_dead = 0.05f;
float autoaim_yaw_dead = 0.05f;

float autoaim_yaw_offset = 0;

float autoaim_pitch_limit = 5.0f;
float autoaim_yaw_limit = 10.0f;

/**
  * @brief          MiniPC task
  * @param          NULL
  * @retval         NULL
  */
void MiniPC_Task(void const* argument) {
    for (;;) {
        while (!GLOBAL_INIT_FLAG) {
            osDelay(1);
        }

        MiniPC_SendHeartPacket();
        osDelay(MINI_PC_TASK_PERIOD);
    }
}

/**
  * @brief      Gets the pointer to the MiniPC data object
  * @param      NULL
  * @retval     Pointer to MiniPC data object
  */
MiniPC_MiniPCControlTypeDef* MiniPC_GetMiniPCControlDataPtr() {
    return &MiniPC_MiniPCContrlData;
}

/**
* @brief      Init minipc data
* @param      NULL
* @retval     NULL
*/
void MiniPC_ControlInit() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();

    Filter_LowPassInit(0.5, &minipc->pitch_lowfil_param);
    Filter_LowPassInit(0.5, &minipc->yaw_lowfil_param);
    Filter_LowPassInit(0.5, &minipc->dis_lowfil_param);

    minipc->enable_aim_output = 1;
    minipc->enable_prediction = 1;
    MiniPC_ResetKFParam();
}

/**
* @brief      Reset Kf param
* @param      NULL
* @retval     NULL
*/
void MiniPC_ResetKFParam() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    float angle, speed;

    minipc->enable_aim_output = 1;
    minipc->enable_prediction = 1;

    static float X_YAW_Hat[2];
    angle = imu->angle.yaw - minipc->yaw_angle;
    speed = MiniPC_CV_CalInitSpeed(-minipc->yaw_angle);
    X_YAW_Hat[0] = angle;
    X_YAW_Hat[1] = speed;

    static float X_PITCH_Hat[2];
    angle = imu->angle.pitch + minipc->pitch_angle;
    speed = 0;
    X_PITCH_Hat[0] = angle;
    X_PITCH_Hat[1] = speed;

    static float Q_YAW_Init[4] = {
        0.02f, 0,
        0.0f, 40.0f};

    static float Q_PITCH_Init[4] = {
        0.01f, 0,
        0.0f, 40.0f};
    static float R_YAW_Init[1] = {
        0.5f};

    static float R_PITCH_Init[1] = {
        0.5f};
    static float P_Init[4] = {
        1, 0,
        0, 1};
    static float F_Init[4] = {
        1, dt,
        0, 1};
    static float H_Init[2] = {
        1,
        0};
    minipc->kf_pitch.UseAutoAdjustment = 0;
    minipc->kf_yaw.UseAutoAdjustment = 0;

    static float state_min_variance[2] = {0.003, 0.005};

    Kalman_FilterInit(&minipc->kf_pitch, 2, 0, 1);
    Kalman_FilterInit(&minipc->kf_yaw, 2, 0, 1);

    //CVKF for Yaw Angle:
    memcpy(minipc->kf_yaw.xhat_data, X_YAW_Hat, sizeof(X_YAW_Hat));
    memcpy(minipc->kf_yaw.H_data, H_Init, sizeof(H_Init));
    memcpy(minipc->kf_yaw.F_data, F_Init, sizeof(F_Init));
    memcpy(minipc->kf_yaw.P_data, P_Init, sizeof(P_Init));
    memcpy(minipc->kf_yaw.R_data, R_YAW_Init, sizeof(R_YAW_Init));
    memcpy(minipc->kf_yaw.Q_data, Q_YAW_Init, sizeof(Q_YAW_Init));
    memcpy(minipc->kf_yaw.StateMinVariance, state_min_variance, sizeof(state_min_variance));

    //CVKF for Pitch Angle:
    memcpy(minipc->kf_pitch.xhat_data, X_PITCH_Hat, sizeof(X_PITCH_Hat));
    memcpy(minipc->kf_pitch.H_data, H_Init, sizeof(H_Init));
    memcpy(minipc->kf_pitch.F_data, F_Init, sizeof(F_Init));
    memcpy(minipc->kf_pitch.P_data, P_Init, sizeof(P_Init));
    memcpy(minipc->kf_pitch.R_data, R_PITCH_Init, sizeof(R_PITCH_Init));
    memcpy(minipc->kf_pitch.Q_data, Q_PITCH_Init, sizeof(Q_PITCH_Init));
    memcpy(minipc->kf_pitch.StateMinVariance, state_min_variance, sizeof(state_min_variance));
}

/**
* @brief      Set gimbal following mode
* @param      mode: MiniPC target follow mode enum
* @retval     NULL
*/
void MiniPC_SetFollowMode(MiniPC_TargetFollowModeEnum mode) {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();

    minipc->target_state = mode;
}

/**
* @brief      Change aiming mode
* @param      mode: MiniPC aim mode enum
* @retval     NULL
*/
void MiniPC_ChangeAimMode(MiniPC_AutoAimModeEnum mode) {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();

    minipc->aim_mode = mode;
    minipc_data->mode = minipc->aim_mode;
}

/**
* @brief      MiniPC auto aim decode control
* @param      NULL
* @retval     NULL
*/
void MiniPC_CalcAutoAim() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();

    // Set following mode
    MiniPC_SetTargetFollowMode();
    MiniPC_SetGimbalRef();
}

/**
* @brief      Set the state of the target being recognized 
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetTargetFollowMode() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();

    uint32_t now = HAL_GetTick();
    if (now - minipc->get_target_time <= Const_MiniPC_Follow_Target_Time) {
        MiniPC_SetFollowMode(MiniPC_TARGET_FOLLOWING);
    } else if (now - minipc->get_target_time >= Const_MiniPC_Lost_Target_Time) {
        MiniPC_SetFollowMode(MiniPC_TARGET_LOST);
    }
}

/**
* @brief      Add pitch axis trajectory compensation
* @param      angle_pitch: Set pitch axis angle
* @retval     float: Pitch axis angle after compensation
*/
float MiniPC_AddPitchOffset(float angle_pitch) {
    float offset = 0.0f;
    if (angle_pitch > 0) {
        offset = 10.0f;
    } else if (angle_pitch > -10.0f) {
        offset = 5.0f;
    } else {
        offset = 0.1f;
    }
    return offset;
}

/**
* @brief      Update minipc data
* @param      NULL
* @retval     NULL
*/
void MiniPC_UpdateControlData() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();

    if (minipc_data->new_data_flag != 1)
        return;

    if (minipc_data->is_get_target == 1) {
        minipc->get_target_time = HAL_GetTick();
    }

    // Update data when get target
    // Otherwise keep last data
    minipc->yaw_angle = Filter_LowPass(minipc_data->yaw_angle, &minipc->yaw_lowfil_param, &minipc->yaw_lowfil);
    minipc->pitch_angle = Filter_LowPass(minipc_data->pitch_angle, &minipc->pitch_lowfil_param, &minipc->pitch_lowfil);
    minipc->distance = Filter_LowPass(minipc_data->distance, &minipc->dis_lowfil_param, &minipc->dis_lowfil);

    LimitMax(minipc->yaw_angle, autoaim_yaw_limit);
    LimitMax(minipc->pitch_angle, autoaim_pitch_limit);

    // Update cvkf-mode when get new measurements!
    minipc->kf_yaw.MeasuredVector[0] = imu->angle.yaw - minipc->yaw_angle;
    minipc->kf_pitch.MeasuredVector[0] = imu->angle.pitch + minipc->pitch_angle;

    minipc_data->new_data_flag = 0;
}

/**
  * @brief      Forecast N cycles without variance iteration
  * @param      kf: Kalman filter structure
  * @param      nT: Forecast period
  * @retval     NULL
  */
float MiniPC_PredictNT(Kalman_KalmanTypeDef* kf, uint32_t nT) {
    float pre_time = kf->F_data[1] * nT;
    float pre_angle = kf->FilteredValue[0];
    float pre_speed = kf->FilteredValue[1];

    if (fabs(pre_speed) >= 3.5f) {
        pre_angle += pre_speed * pre_time;
    }
    return pre_angle;
}

/**
* @brief      Set CV mode speed
* @param      NULL
* @retval     NULL
*/
float MiniPC_CV_CalInitSpeed(float delta_err_angle) {
    static float delta_angle = 0.0f;
    if (fabs(delta_err_angle) <= delta_angle) {
        return 0.0f;
    } else if (delta_err_angle > delta_angle) {
        return 10.0f;
    } else if (delta_err_angle < -delta_angle) {
        return -10.0f;
    }
    return 0.0f;
}

/**
* @brief      Kalman prediction
* @param      NULL
* @retval     NULL
*/
void MiniPC_KalmanPrediction() {
    static MiniPC_TargetFollowModeEnum last_target_state = MiniPC_TARGET_LOST;
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();

    float angle_yaw = imu->angle.yaw - minipc->yaw_angle;
    float angle_pitch = imu->angle.pitch + minipc->pitch_angle;

    if (minipc->target_state == MiniPC_TARGET_FOLLOWING && (last_target_state == MiniPC_TARGET_LOST)) {
        MiniPC_ResetKFParam();

        minipc->yaw_ref_filtered = angle_yaw;
        minipc->pitch_ref_filtered = angle_pitch;
    }

    else if ((minipc->target_state == MiniPC_TARGET_FOLLOWING) && (last_target_state == MiniPC_TARGET_FOLLOWING)) {
        // Always get the new Measurement For CVKF
        if (minipc->enable_prediction == 1) {
            Kalman_FilterUpdate(&minipc->kf_pitch);
            Kalman_FilterUpdate(&minipc->kf_yaw);
            minipc->yaw_ref_filtered = minipc->kf_yaw.FilteredValue[0];
            minipc->pitch_ref_filtered = minipc->kf_pitch.FilteredValue[0];
        }
    }

    else {
        // No Targets: Close CVKF
        MiniPC_ResetKFParam();
        minipc->yaw_ref_filtered = angle_yaw;
        minipc->pitch_ref_filtered = angle_pitch;
    }

    last_target_state = minipc->target_state;
}

/**
* @brief      Set gimbal autoaim reference
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetAutoAimRef() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    float after_predict_yaw;
    float after_predict_pitch;

    MiniPC_KalmanPrediction();

    if (minipc->enable_prediction == 1) {
        after_predict_yaw = MiniPC_PredictNT(&minipc->kf_yaw, CVKF_NT_YAW);
        after_predict_pitch = MiniPC_PredictNT(&minipc->kf_pitch, CVKF_NT_PITCH);
    }

    static float ref_cvkf_yaw_angle = 0.0f;
    static float ref_cvkf_pitch_angle = 0.0f;

    if (fabs(ref_cvkf_yaw_angle - after_predict_yaw) > autoaim_yaw_dead) {
        ref_cvkf_yaw_angle = after_predict_yaw;
    }
    if (fabs(ref_cvkf_pitch_angle - after_predict_pitch) > autoaim_pitch_dead) {
        ref_cvkf_pitch_angle = after_predict_pitch;
    }

    float delta_predict = after_predict_yaw - minipc->yaw_ref_filtered;

    if (fabs(delta_predict) < 0.5f)
        autoaim_yaw_offset = 0;
    else if (delta_predict >= 3.0f)
        autoaim_yaw_offset = 4.0f;
    else if (delta_predict >= 1.0f)
        autoaim_yaw_offset = (delta_predict - 1.0f) * 4.0f / 2.5f;
    else if (delta_predict <= -3.0f)
        autoaim_yaw_offset = -4.0;
    else if (delta_predict <= -1.0f)
        autoaim_yaw_offset = (delta_predict + 1.0f) * 4.0f / 2.5f;
    else
        autoaim_yaw_offset = 0;

    Gimbal_SetYawAutoRef(ref_cvkf_yaw_angle + autoaim_yaw_offset);
    Gimbal_SetPitchAutoRef(ref_cvkf_pitch_angle + Const_autoaim_pitch_offset);
}

/**
* @brief      Set gimbal reference
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetGimbalRef() {
    MiniPC_MiniPCControlTypeDef* minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef* minipc_data = MiniPC_GetMiniPCDataPtr();
    INS_IMUDataTypeDef* imu = Ins_GetIMUDataPtr();
    Gimbal_GimbalTypeDef* gimbal = Gimbal_GetGimbalControlPtr();

    if ((minipc->enable_aim_output) && (minipc->target_state == MiniPC_TARGET_FOLLOWING) && (gimbal->mode.present_mode == Gimbal_ARMOR)) {
        MiniPC_SetAutoAimRef();
    }

    else if ((minipc->enable_aim_output) && (minipc->target_state == MiniPC_TARGET_FOLLOWING) && (gimbal->mode.present_mode == Gimbal_BIG_ENERGY)) {
        Gimbal_SetYawAutoRef(imu->angle.yaw - minipc->yaw_ref_filtered + Const_energy_yaw_offset);
        Gimbal_SetPitchAutoRef(imu->angle.pitch + minipc->pitch_ref_filtered + Const_energy_pitch_offset);
    } else if ((minipc->enable_aim_output) && (minipc->target_state == MiniPC_TARGET_FOLLOWING) && (gimbal->mode.present_mode == Gimbal_SMALL_ENERGY)) {
        Gimbal_SetYawAutoRef(imu->angle.yaw - minipc->yaw_ref_filtered + Const_energy_yaw_offset);
        Gimbal_SetPitchAutoRef(imu->angle.pitch + minipc->pitch_ref_filtered + Const_energy_pitch_offset);
    }

    else
        return;
}

#endif
