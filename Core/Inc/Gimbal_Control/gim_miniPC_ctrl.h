/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : gim_miniPC_ctrl.h
 *  Description  : This file contains MiniPC control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-10 11:03:12
 *  LastEditTime : 2021-07-22 12:36:23
 */

#ifndef GIM_MINIPC_CTRL_H
#define GIM_MINIPC_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_CTRL_MINIPC)

#include "minipc_periph.h"
#include "math_alg.h"
#include "filter_alg.h"
#include "kalman_alg.h"

typedef enum {
    MiniPC_TARGET_LOST = 0u,
    MiniPC_TARGET_FOLLOWING = 1u
} MiniPC_TargetFollowModeEnum;

typedef enum {
    MiniPC_ARMOR = 0u,
    MiniPC_BIG_BUFF = 1u,
    MiniPC_SMALL_BUFF = 2u
} MiniPC_AutoAimModeEnum;

typedef struct {
    uint8_t enable_aim_output;
    uint8_t enable_prediction;

    float distance;
    float yaw_angle;
    float pitch_angle;

    float yaw_ref_filtered, pitch_ref_filtered;

    Kalman_KalmanTypeDef kf_yaw, kf_pitch;

    uint32_t get_target_time;
    MiniPC_AutoAimModeEnum aim_mode;
    MiniPC_TargetFollowModeEnum target_state;
} MiniPC_MiniPCControlTypeDef;

MiniPC_MiniPCControlTypeDef* MiniPC_GetMiniPCControlDataPtr(void);
void MiniPC_ControlInit(void);
void MiniPC_ChangeAimMode(MiniPC_AutoAimModeEnum mode);
void MiniPC_CalcAutoAim(void);
void MiniPC_SetFollowMode(MiniPC_TargetFollowModeEnum mode);
float MiniPC_CV_CalInitSpeed(float delta_err_angle);
void MiniPC_SetTargetFollowMode(void);
void MiniPC_UpdateControlData(void);
void MiniPC_SetGimbalRef(void);
void MiniPC_SetAutoAimRef(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
