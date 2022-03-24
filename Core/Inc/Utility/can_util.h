/*
 * @Project      : RM_Infantry_Neptune
 * @FilePath     : \infantry_-neptune\Core\Inc\Utility\can_util.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2021-12-31 17:37:14
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-03-24 19:52:28
 */


#ifndef CAN_UTIL_H
#define CAN_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_UTIL_CAN)

#include "can.h"

void Can_ErrorHandler(uint32_t ret);
void Can_InitTxHeader(CAN_TxHeaderTypeDef* pheader, uint32_t stdid, uint32_t extid, uint32_t dlc);
void Can_InitFilterAndStart(CAN_HandleTypeDef* phcan);
void Can_SendMessage(CAN_HandleTypeDef* phcan, CAN_TxHeaderTypeDef* pheader, uint8_t txdata[]);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* phcan);
void Can_RxMessageCallback(CAN_HandleTypeDef* phcan, CAN_RxHeaderTypeDef* rxheader, uint8_t rxdata[]);

#endif

#ifdef __cplusplus
}
#endif

#endif
