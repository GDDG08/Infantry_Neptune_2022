/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : sensor_periph.h
 *  Description  : This file contains analog sensor function(Only for Super Cap)
 *  LastEditors  : ����ؼ���ᶯ��
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-08 09:15:30
 */

#ifndef SENSOR_PERIPH_H
#define SENSOR_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_PERIPH_SENSOR)

#include "adc_util.h"
#include "filter_alg.h"

typedef struct {
    float CapVoltage,    // Super Cap Voltage
        VccVoltage,      // Referee Supply Voltage
        ChasissVoltage;  // Chassis Voltage
    /*Super Cap data*/
} Sen_CAPBasisValueTypeDef;

typedef struct {
    float CapRestEnergy;  // Cap Rest Energy
    uint8_t CapPercent;   // Cap Percent Energy
    /*Power data*/
} Sen_PowerValueTypeDef;

/* filter data*/
typedef struct {
    Filter_LowPassParamTypeDef CapVoltageParam,
        VccVoltageParam,
        ChasissVoltageParam;
    Filter_LowPassTypeDef CapVoltage,
        VccVoltage,
        ChasissVoltage;
} Sen_FilterTypeDef;

extern Sen_CAPBasisValueTypeDef Sen_basisValue;
extern Sen_PowerValueTypeDef Sen_powerValue;
extern Sen_FilterTypeDef Sen_Filter;

Sen_PowerValueTypeDef* Sen_GetPowerDataPtr(void);
Sen_CAPBasisValueTypeDef* Sen_GetBasisDataPtr(void);
void Sensor_Decode(void);
void Sen_Filtering(Sen_FilterTypeDef* Sen_Filter, Sen_CAPBasisValueTypeDef* Sen_basisValue);
void Sen_Init(void);

#endif

#ifdef __cplusplus

#endif

#endif
