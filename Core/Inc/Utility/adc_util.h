/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : adc_util.h
 *  Description  : This file contains the ADC functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-10 11:03:12
 *  LastEditTime : 2021-06-18 21:48:30
 */

#ifndef ADC_UTIL_H
#define ADC_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_UTIL_ADC)

#include "adc.h"
#include "string.h"

extern uint32_t Adc_valueBuf[30];
extern float Adc_decodeBuf[30];

void Adc_Init(void);
void Adc_GetData(void);
void Adc_Decode(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
