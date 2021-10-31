/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \infantry_-neptune\Core\Inc\Common_Contrrol\debug_BTlog.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-31 09:17:07
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-31 09:43:49
 */

#ifndef DEBUG_BTLOG_H
#define DEBUG_BTLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_DEBUG_BTLOG)

#include "uart_util.h"
#include "buff_lib.h"
#include "const.h"
#include "buscomm_ctrl.h"

extern UART_HandleTypeDef* Const_BTlog_UART_HANDLER;

void BTlog_Init(void);
void BTlog_Send(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
