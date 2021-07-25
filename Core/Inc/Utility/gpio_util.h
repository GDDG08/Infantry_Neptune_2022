/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : gpio_util.h
 *  Description  : This file contains the GPIO functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 04:13:44
 */


#ifndef GPIO_UTIL_H
#define GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "main.h"
#include "configure.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#define RISE_TRIGGER    0xAA
#define DOWN_TRIGGER    0xBB
#define KEY_FUNC_EVENT_ID   0xA1
#define KEY_BACK_EVENR_ID   0x12

typedef struct {
    GPIO_TypeDef* gpio_handle;
    uint16_t gpio_pin;
    
    uint16_t event_id;

    uint32_t tick;
    uint32_t last_tick;
    GPIO_PinState pin_state;
} GPIO_GPIOTypeDef;

#if __FN_IF_ENABLE(__FN_INFANTRY_GIMBAL)

extern GPIO_GPIOTypeDef *LASER;
extern GPIO_GPIOTypeDef *BULLET_CHARGING;
extern GPIO_GPIOTypeDef *CS_ACCEL;
extern GPIO_GPIOTypeDef *CS_GYRO;
extern GPIO_GPIOTypeDef *IST8310_RST;

extern GPIO_GPIOTypeDef *IST8310_DRDY;
extern GPIO_GPIOTypeDef *BMI_INT1;
extern GPIO_GPIOTypeDef *BMI_INT3;

extern GPIO_GPIOTypeDef *KEY_FUNC;
extern GPIO_GPIOTypeDef *KEY_BACK;

extern GPIO_GPIOTypeDef *CODE1;
extern GPIO_GPIOTypeDef *CODE2;
extern GPIO_GPIOTypeDef *CODE3;

#endif

#if __FN_IF_ENABLE(__FN_SUPER_CAP)

extern GPIO_GPIOTypeDef *BOOST;
extern GPIO_GPIOTypeDef *BUCK;
extern GPIO_GPIOTypeDef *CAP;

#endif

#if __FN_IF_ENABLE(__FN_INFANTRY_CHASSIS)

#endif

void GPIO_IRQCallback(uint16_t GPIO_Pin);
void GPIO_Set(GPIO_GPIOTypeDef *gpio);
void GPIO_Reset(GPIO_GPIOTypeDef *gpio);
uint32_t GPIO_GetTriggerTick(GPIO_GPIOTypeDef *gpio);
GPIO_PinState GPIO_ReadPin(GPIO_GPIOTypeDef *gpio);

extern osMessageQId Key_QueueHandle;

#ifdef __cplusplus
}
#endif

#endif
