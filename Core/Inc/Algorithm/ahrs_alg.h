/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : This file contains AHRS algorithm functions
 *  Description  : ahrs_alg.h
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-10 23:36:27
 *  LastEditTime : 2021-07-09 00:51:40
 */


#ifndef AHRS_ALG_H
#define AHRS_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "math_alg.h"


void AHRS_Init(float quat[4]);
void AHRS_GetAngle(float q[4], float *yaw, float *pitch, float *roll);
void AHRS_MahonyUpdate(float q[4], float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void AHRS_MahonyUpdateIMU(float q[4], float gx, float gy, float gz, float ax, float ay, float az);
void AHRS_MadgwickUpdate(float q[4], float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void AHRS_MadgwickUpdateIMU(float q[4], float gx, float gy, float gz, float ax, float ay, float az);
    
#ifdef __cplusplus
}
#endif

#endif
