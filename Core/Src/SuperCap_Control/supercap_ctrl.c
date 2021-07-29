/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : supercap_ctrl.c
 *  Description  : This file contains cap control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-26 17:15:45
 */

#include "supercap_ctrl.h"

#if __FN_IF_ENABLE(__FN_SUPER_CAP)

#include "buscomm_ctrl.h"
#include "cmsis_os.h"

#include "const.h"

#define SUPER_CAP_PERIOD 20

CAP_ControlValueTypeDef Cap_ControlState;

/**
  * @brief          SuperCap task
  * @param          NULL
  * @retval         NULL
  */
void SuperCap_Task(void const* argument) {
    for (;;) {
        while (!GLOBAL_INIT_FLAG) {
            osDelay(1);
        }
        Cap_Control();
        osDelay(SUPER_CAP_PERIOD);
    }
}

/**
 * @brief      Set charge current
 * @param      cur: cap charge current (ma)
 * @retval     NULL
 */
void Cap_SetChargeCurrent(float cur) {
    /* Set charging dead zone */
    float current = cur;
    if (current <= 0.1f)
        Dac_SetCurrent(0.0f);
    else if (current >= 5.0f)
        Dac_SetCurrent(5.0f);
    else
        Dac_SetCurrent(current);
}

/**
 * @brief      Cap Initialize control function
 * @param      NULL
 * @retval     NULL
 */
void Cap_Init() {
    CAP_ControlValueTypeDef* capvalue = Cap_GetCapControlPtr();

    Cap_SetChargeCurrent(0);
    /* set init current */
    GPIO_Reset(CAP);

    GPIO_Reset(BUCK);
    GPIO_Reset(BOOST);
    /* set init state   */

    capvalue->cap_state = CAP_MODE_OFF;
    capvalue->power_limit = 40.0f;
    capvalue->power_path = Power_PATH_REFEREE;
    capvalue->charge_state = CAP_CHARGE_OFF;

    HAL_Delay(2000);
}

/**
  * @brief      Gets the pointer to the cap control data object
  * @param      NULL
  * @retval     Pointer to cap control data object
  */
CAP_ControlValueTypeDef* Cap_GetCapControlPtr() {
    return &Cap_ControlState;
}

/**
 * @brief      Judge capacitor state
 * @param      NULL
 * @retval     NULL
 */
void Cap_JudgeCapState() {
    Sen_CAPBasisValueTypeDef* basisvalue = Sen_GetBasisDataPtr();
    CAP_ControlValueTypeDef* capvalue = Cap_GetCapControlPtr();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    if (basisvalue->CapVoltage < Cap_MinVoltage && basisvalue->CapVoltage >= 0) {
        capvalue->cap_state = CAP_MODE_OFF;
        buscomm->cap_state = SUPERCAP_MODE_OFF;
    } else if (basisvalue->CapVoltage <= 0) {
        capvalue->cap_state = CAP_MODE_ERROR;
        buscomm->cap_state = SUPERCAP_MODE_ERROR;
    } else if (basisvalue->CapVoltage >= Cap_AvailableVoltage) {
        capvalue->cap_state = CAP_MODE_ON;
        buscomm->cap_state = SUPERCAP_MODE_ON;
    }
}

/**
 * @brief      Change Control mode
 * @param      NULL
 * @retval     NULL
 */
void Cap_CapCharge() {
    CAP_ControlValueTypeDef* capvalue = Cap_GetCapControlPtr();
    Sen_PowerValueTypeDef* sendata = Sen_GetPowerDataPtr();
    Sen_CAPBasisValueTypeDef* basisdata = Sen_GetBasisDataPtr();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    if (buscomm->cap_charge_mode == SUPERCAP_UNCHARGE) {
        GPIO_Reset(BUCK);
        Cap_SetChargeCurrent(0);
    } else if (buscomm->cap_charge_mode == SUPERCAP_CHARGE) {
        capvalue->power_limit = 0.7f * (float)buscomm->power_limit;

        if (basisdata->CapVoltage <= 7.0f) {
            Cap_SetChargeCurrent(4.0f);
        } else if (basisdata->CapVoltage <= 12.0f) {
            Cap_SetChargeCurrent(3.0f);
        } else if (basisdata->CapVoltage >= 26.5f) {
            Cap_SetChargeCurrent(0);
        } else {
            Cap_SetChargeCurrent(capvalue->power_limit / basisdata->CapVoltage);
        }
        GPIO_Set(BUCK);
    }
}

/**
 * @brief      Change Control mode
 * @param      NULL
 * @retval     NULL
 */
void Cap_ChangePowerPath(POWER_PathEnum path) {
    CAP_ControlValueTypeDef* capvalue = Cap_GetCapControlPtr();

    if (path == Power_PATH_CAP && (capvalue->cap_state == CAP_MODE_ERROR || capvalue->cap_state == CAP_MODE_OFF))
        GPIO_Reset(CAP);
    else if (path == Power_PATH_CAP && capvalue->cap_state == CAP_MODE_ON)
        GPIO_Set(CAP);
    else if (path == Power_PATH_REFEREE)
        GPIO_Reset(CAP);
}

/**
 * @brief      Super Cap control function
 * @param      NULL
 * @retval     NULL
 */
void Cap_Control() {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    Sensor_Decode();
    // sensor data decode
    Cap_JudgeCapState();
    // Judege Cap state
    Cap_CapCharge();

    if (buscomm->cap_mode == SUPERCAP_CTRL_ON)
        Cap_ChangePowerPath(Power_PATH_CAP);

    else if (buscomm->cap_mode == SUPERCAP_CTRL_OFF)
        Cap_ChangePowerPath(Power_PATH_REFEREE);
}

#endif
