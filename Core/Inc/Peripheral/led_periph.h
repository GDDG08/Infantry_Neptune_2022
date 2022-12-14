/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : led_periph.h
 *  Description  : This file contains the led periph control functions
 *  LastEditors  : ????ؼ???ᶯ??
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-08 10:57:05
 */

#ifndef LED_PERIPH_H
#define LED_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#if __FN_IF_ENABLE(__FN_PERIPH_LED)

#include "stm32f4xx_hal.h"
#include "gpio.h"

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LED_LEDStateEnum;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    LED_LEDStateEnum state;
    GPIO_PinState off_pin_state, on_pin_state;
} LED_LEDTypeDef;

void LED_InitAllLEDs(void);
void LED_InitLED(LED_LEDTypeDef* led, GPIO_TypeDef* port, uint16_t pin, GPIO_PinState off_pin_state, GPIO_PinState on_pin_state, LED_LEDStateEnum init_state);
LED_LEDStateEnum LED_GetLEDState(LED_LEDTypeDef* led);
void LED_SetLEDState(LED_LEDTypeDef* led, LED_LEDStateEnum state);
void LED_AllOff(void);

#endif

#endif

#ifdef __cplusplus
}
#endif
