/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : buscomm_cmd.h
 *  Description  : This file is for idiot Can communication
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-09 03:52:53
 *  LastEditTime : 2021-07-11 08:14:24
 */

#ifndef BUSCOMM_CMD_H
#define BUSCOMM_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#include "stm32f4xx_hal.h"

extern const uint32_t CMD_SET_YAW_ANGLE_BASIC_DATA;
extern const uint32_t CMD_SET_17MM_DATA;

extern const uint32_t CMD_SET_MODE;
extern const uint32_t CMD_SET_GIMBAL_DATA;
extern const uint32_t CMD_SET_IMU_YAW;
extern const uint32_t CMD_SET_CHA_REF;

extern const uint32_t CMD_SEND_CAP_STATE;

extern const uint32_t CMD_CHASSIS_SEND_PACK_1;
extern const uint32_t CMD_CHASSIS_SEND_PACK_2;

extern const uint32_t CMD_GIMBAL_SEND_PACK_1;
extern const uint32_t CMD_GIMBAL_SEND_PACK_2;
extern const uint32_t CMD_GIMBAL_SEND_PACK_3;
extern const uint32_t CMD_GIMBAL_SEND_PACK_4;

extern const uint32_t CMD_SUPERCAP_SEND_PACK_1;

typedef struct {
    uint32_t cmd_id;
    void (*bus_func)(uint8_t buff[]);
} BusCmd_TableEntry;

extern BusCmd_TableEntry Buscmd_Receive[8];
extern BusCmd_TableEntry Buscmd_GimSend[4];
extern BusCmd_TableEntry Buscmd_ChaSend[2];
extern BusCmd_TableEntry Buscmd_CapSend[1];

#endif

#ifdef __cplusplus
}
#endif
