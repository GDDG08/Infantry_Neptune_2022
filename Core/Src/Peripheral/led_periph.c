/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : led_periph.c
 *  Description  : This file contains the led periph control functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 11:06:13
 */

#include "led_periph.h"

#if __FN_IF_ENABLE(__FN_PERIPH_LED)

LED_LEDTypeDef LED_LED1, LED_LED2;

/**
  * @brief      Initialize all onboard LEDs
  * @param      NULL
  * @retval     NULL
  */
void LED_InitAllLEDs() {
    //    LED_InitLED(&LED_LED1,  GPIOC, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET, LED_OFF);
    //    LED_InitLED(&LED_LED2,  GPIOA, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET, LED_OFF);
}

/**
  * @brief      Read LED status
  * @param      led: Pointer to LED object
  * @retval     LED status
  */
LED_LEDStateEnum LED_GetLEDState(LED_LEDTypeDef* led) {
    return led->state;
}

/**
  * @brief      Set LED status
  * @param      led: Pointer to LED object
  * @param      state: LED status
  * @retval     NULL
  */
void LED_SetLEDState(LED_LEDTypeDef* led, LED_LEDStateEnum state) {
    if (state == LED_OFF) {
        HAL_GPIO_WritePin(led->port, led->pin, led->off_pin_state);
    } else if (state == LED_ON) {
        HAL_GPIO_WritePin(led->port, led->pin, led->on_pin_state);
    }
    led->state = state;
}

/**
  * @brief      Turn off all LEDs
  * @param      NULL
  * @retval     NULL
  */
void LED_AllOff() {
    LED_SetLEDState(&LED_LED1, LED_OFF);
    LED_SetLEDState(&LED_LED2, LED_OFF);
}

/**
  * @brief      Initialize single LED
  * @param      led: Pointer to LED object
  * @param      port: Led corresponding GPIO port number
  * @param      pin: Led corresponding GPIO pin number
  * @param      off_pin_state: GPIO status corresponding to led off
  * @param      on_pin_state: Led on corresponds to GPIO status
  * @param      init_state: Led initial state
  * @retval     NULL
  */
void LED_InitLED(LED_LEDTypeDef* led, GPIO_TypeDef* port, uint16_t pin, GPIO_PinState off_pin_state, GPIO_PinState on_pin_state, LED_LEDStateEnum init_state) {
    led->port = port;
    led->pin = pin;
    led->off_pin_state = off_pin_state;
    led->on_pin_state = on_pin_state;
    LED_SetLEDState(led, init_state);
}

#endif
