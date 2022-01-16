/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : buff_lib.h
 *  Description  : This file contains auxiliary functions related to byte buffer operation
 *  LastEditors  : ����ؼ���ᶯ��
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-08 03:36:04
 */

#ifndef BUFF_LIB_H
#define BUFF_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "string.h"

float buff2float(uint8_t* buff);
void float2buff(float f, uint8_t* buff);
int16_t buff2i16(uint8_t* buff);
uint16_t buff2ui16(uint8_t* buff);
void ui162buff(uint16_t u, uint8_t* buff);
void i162buff(int16_t u, uint8_t* buff);
uint32_t buff2ui32(uint8_t* buff);
void ui322buff(uint32_t u, uint8_t* buff);

#ifdef __cplusplus
}
#endif

#endif
