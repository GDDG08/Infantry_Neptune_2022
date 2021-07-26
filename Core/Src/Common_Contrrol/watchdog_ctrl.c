/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : watchdog_ctrl.c
 *  Description  : This is my lovely dog's home
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-11 09:12:11
 */

#include "watchdog_ctrl.h"

#include "buscomm_ctrl.h"
#include "supercap_ctrl.h"
#include "remote_periph.h"
#include "referee_periph.h"
#include "motor_periph.h"
#include "miniPC_periph.h"
#include "cha_gimbal_ctrl.h"
#include "cha_chassis_ctrl.h"
#include "gim_login_ctrl.h"

#define WATCHDOG_TASK_PERIOD 1

/**
  * @brief          WatchDog task
  * @param          NULL
  * @retval         NULL
  */
void WatchDog_Task(void const* argument) {
    for (;;) {
        WatchDog_FeedDog();
        osDelay(WATCHDOG_TASK_PERIOD);
    }
}

void WatchDog_FeedDog() {
#if __FN_IF_ENABLE(__FN_INFANTRY_CHASSIS)
    if (BusComm_IsBusCommOffline()) {
        GimbalYaw_SetGimbalYawControlState(0);
        GimbalYaw_SetGimbalYawOutputState(0);
        Chassis_SetMode(Chassis_MODE_STOP);
    } else {
        GimbalYaw_SetGimbalYawControlState(1);
        GimbalYaw_SetGimbalYawOutputState(1);
        //            Chassis_SetChassisControlState(1);
        //            Chassis_SetChassisOutputState(1);
    }
#endif

#if __FN_IF_ENABLE(__FN_INFANRTY_GIMBAL)
    if (LOGIN_ON_FLAG == 0) {
    }
#endif
}
