/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : pid_alg.h
 *  Description  : This file contains PID algorithm function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-09 04:07:00
 */

#ifndef PID_ALG_H
#define PID_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#include "math_alg.h"
#include "filter_alg.h"
#include "stddef.h"
#include "string.h"

typedef enum {
    PID_POSITION = 0u,
    PID_DELTA = 1u
} PID_ModeEnum;

typedef struct {
    float ref;
    float fdb;
    float err[3];
    float err_lim;          // Integral anti-windup 
    float err_fdf[3];       // Feedforard
    float out_fdf;          // Feedforard out put
    float sum;
    float output;

    Filter_LowPassTypeDef d_fil;
    Filter_LowPassTypeDef delta_fil;

    Filter_LowPassTypeDef kf1_fil;
    Filter_LowPassTypeDef kf2_fil;
    float err_watch;
} PID_PIDTypeDef;

typedef struct {
    PID_ModeEnum pid_mode;

    // PID
    float kp;                   // pid param
    float ki;
    float kd;
    
    float sum_max;              // sum limit    
    float output_max;           // out limit
    
    Filter_LowPassParamTypeDef d_fil_param;         // Kd filter
    Filter_LowPassParamTypeDef delta_fil_param;     // delta filter ref filter

    // Feedforward
    float kf_1;                                     // feedforward param
    float kf_2;         
    
    Filter_LowPassParamTypeDef kf1_fil_param;      // feed forawrd filter
    Filter_LowPassParamTypeDef kf2_fil_param;
} PID_PIDParamTypeDef;

void PID_InitPIDParam(PID_PIDParamTypeDef* pparam, float kp, float ki, float kd, float sum_max, float output_max, 
                      float kd_fil_param, float delta_fil_param, float kf_1, float kf_2, float kf1_fil_param, 
                      float kf2_fil_param,PID_ModeEnum pid_mode);
float PID_GetPIDRef(PID_PIDTypeDef* pid);
void PID_SetPIDRef(PID_PIDTypeDef* pid, float ref);
void PID_AddPIDRef(PID_PIDTypeDef* pid, float inc);
float PID_GetPIDFdb(PID_PIDTypeDef* pid);
void PID_SetPIDFdb(PID_PIDTypeDef* pid, float fdb);
float PID_GetPIDOutput(PID_PIDTypeDef* pid);
void PID_ClearPID(PID_PIDTypeDef* pid);
void PID_CalcPID(PID_PIDTypeDef* pid, PID_PIDParamTypeDef* para);

#ifdef __cplusplus
}
#endif

#endif
