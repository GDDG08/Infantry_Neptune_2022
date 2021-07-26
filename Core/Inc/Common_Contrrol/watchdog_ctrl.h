/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : watchdog_ctrl.h
 *  Description  : This is my lovely dog's home too
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-05-08 10:19:02
 */

#ifndef WATCHDOG_CTRL_H
#define WATCHDOG_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configure.h"

void WatchDog_Task(void const* argument);
void WatchDog_FeedDog(void);

#ifdef __cplusplus
}
#endif

#endif
