/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : This file contains key basic functions
 *  Description  : key_periph.c
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-10 23:49:33
 *  LastEditTime : 2021-07-09 04:41:33
 */

#include "key_periph.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "buscomm_ctrl.h"

#define LONG_PRESS_TIME 1200
#define FLASE_TRIGGER_TIME 80

/**
  * @brief          Get infantry equipment code
  * @param          NULL
  * @retval         Infantry equipment code
  */
uint8_t Key_GetEquipCode() {
    uint8_t code = 0x00;

#if __FN_IF_ENABLE(__FN_INFANTRY_GIMBAL)

    if (GPIO_ReadPin(CODE1) == GPIO_PIN_RESET) {
        code |= 0x01;
    }
    if (GPIO_ReadPin(CODE2) == GPIO_PIN_RESET) {
        code |= 0x02;
    }
    if (GPIO_ReadPin(CODE3) == GPIO_PIN_RESET) {
        code |= 0x04;
    }
#else
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    code = buscomm->infantry_code;
#endif

    return code;
}

/**
  * @brief          Key event handler
  * @param          gpio: interrupt header
  * @retval         NULL
  */
void Key_KeyEventHandler(GPIO_GPIOTypeDef* gpio) {
    uint16_t event = 0;
    uint8_t trigger = 0;
    uint32_t trigger_time = 0;
    uint8_t press_event;
    if (gpio->pin_state == GPIO_PIN_SET) {
        trigger = RISE_TRIGGER;
    } else {
        trigger = DOWN_TRIGGER;
    }

    if (trigger == DOWN_TRIGGER) {
        gpio->last_tick = gpio->tick;
    } else if (trigger == RISE_TRIGGER) {
        trigger_time = gpio->tick - gpio->last_tick;

        if (trigger_time <= FLASE_TRIGGER_TIME) {
            return;
        } else if ((trigger_time >= FLASE_TRIGGER_TIME) && (trigger_time <= LONG_PRESS_TIME)) {
            press_event = SHORT_PRESS_EVENT;
        } else if (trigger_time >= LONG_PRESS_TIME) {
            press_event = LONG_PRESS_EVENT;
        }
        event = (uint32_t)((press_event << 8) | (gpio->event_id));
        osStatus ret = osMessagePut(Key_QueueHandle, event, 10);
    }
}
