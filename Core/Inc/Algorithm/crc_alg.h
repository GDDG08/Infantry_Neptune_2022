/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : crc_alg.h
 *  Description  : This file contains crc check function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-07 12:18:46
 */

#ifndef CRC_ALG_H
#define CRC_ALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


extern unsigned char CRC8;
extern uint16_t CRC16;
extern uint16_t CRC16_INIT;
extern unsigned char CRC8_INIT;

typedef enum {
        NOT_MATCH = 0,
        MATCH     = 1
} CRC_MatchEnum;


unsigned char CRC_GetCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength, char ucCRC8);
unsigned int CRC_VerifyCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
void CRC_AppendCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t CRC_GetCRC16CheckSum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
unsigned int CRC_VerifyCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);
void CRC_AppendCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);
CRC_MatchEnum CRC_VerifyIMU_HI229(uint8_t* buff);
void crc16_verify(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes);

#ifdef __cplusplus
}
#endif

#endif

