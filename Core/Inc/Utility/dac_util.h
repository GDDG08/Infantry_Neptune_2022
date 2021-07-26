/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : dac_util.h
 *  Description  : This file contains the DAC functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-11 14:19:14
 *  LastEditTime : 2021-06-18 21:49:42
 */

#ifndef DAC_UTIL_H
#define DAC_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_UTIL_DAC)

#include "dac.h"

typedef enum {
    DAC_OFF = 0,
    DAC_ON = 1
} Dac_DacStateEnum;

typedef struct {
    Dac_DacStateEnum state;
    DAC_HandleTypeDef* hdac;
    uint32_t ch;
    float value;
    uint32_t Dac_DecodeValue;
} Dac_DacHandleTypeDef;

extern Dac_DacHandleTypeDef CurrentDac;

void Dac_StopDac(void);
void Dac_SetCurrent(float value);
void Dac_StopDac(void);
void Dac_DecodeValue(void);
void Dac_Init(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
