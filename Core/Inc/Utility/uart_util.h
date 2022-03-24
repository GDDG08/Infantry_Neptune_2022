/*
 * @Project      : RM_Infantry_Neptune
 * @FilePath     : \infantry_-neptune\Core\Inc\Utility\uart_util.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2021-12-31 17:37:14
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-03-24 19:58:44
 */

#ifndef UART_UTIL_H
#define UART_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_UTIL_UART)

#include "usart.h"

void Uart_RxIdleCallback(UART_HandleTypeDef* huart);
void Uart_SendMessage(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
void Uart_SendMessage_IT(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size);
void Uart_SendMessage_IT_Force(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
void Uart_ErrorHandler(uint32_t ret);
uint16_t Uart_DMACurrentDataCounter(DMA_Stream_TypeDef* dma_stream);
void Uart_InitUartDMA(UART_HandleTypeDef* huart);
void Uart_ReceiveDMA(UART_HandleTypeDef* huart, uint8_t rxdata[], uint32_t size);
void Uart_ReceiveHandler(UART_HandleTypeDef* huart);

#endif

#ifdef __cplusplus
}
#endif

#endif
