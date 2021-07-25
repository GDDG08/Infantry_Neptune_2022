/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : This file contains key basic functions
 *  Description  : key_periph.h
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-10 23:49:57
 *  LastEditTime : 2021-07-11 10:40:20
 */


#ifndef KEY_PERIPH_H
#define KEY_PERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

#include "gpio_util.h"

#define LONG_PRESS_EVENT    0x02
#define SHORT_PRESS_EVENT   0x01

#define INFANTRY_3  3
#define INFANTRY_4  4
#define INFANTRY_5  5
#define INFANTRY_6  6
#define INFANTRY_7  7
#define INFANTRY_8  8

uint8_t Key_GetEquipCode(void);
void Key_KeyEventHandler(GPIO_GPIOTypeDef *gpio);

#ifdef __cplusplus
}
#endif

#endif
