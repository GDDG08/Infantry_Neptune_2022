/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : cha_power_ctrl.c
 *  Description  : This file contains power control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-09 07:37:27
 */

#include "cha_power_ctrl.h"

#if __FN_IF_ENABLE(__FN_CTRL_POWER)

#include "buscomm_ctrl.h"
#include "const.h"

Power_DataTypeDef Power_data;
Power_ControlModeMnum Power_ControlMode;


/**
  * @brief      Power control initialization
  * @param      NULL
  * @retval     NULL
  */
void Power_InitPower() {
    Power_data.power_limit = 40.0f;
    Power_data.warning_power = 30.0f;
    Power_data.warning_power_buff = 40.0f;

    Power_data.no_judge_total_current_limit = 64000.0f;
    Power_data.buffer_total_current_limit = 16000.0f;
    Power_data.power_total_current_limit = 20000.0f;

    Power_ControlMode = POWER_LIMIT;
}


/**
  * @brief      Gets the pointer to the power control data object
  * @param      NULL
  * @retval     Pointer to power control data object
  */
Power_DataTypeDef* Power_GetPowerDataPty() {
    return &Power_data;
}


/**
  * @brief      Force change power contorl mode
  * @param      NULL
  * @retval     NULL
  */
void Power_ForceChangePowerMode(Power_ControlModeMnum mode) {
    Power_ControlMode = mode;
}


/**
  * @brief      Set limit power (linear model)
  * @param      NULL
  * @retval     NULL
  */
void Power_SetLimitPower() {
    Referee_RefereeDataTypeDef *referee = Referee_GetRefereeDataPtr();

    Power_data.power_limit = (float)referee->max_chassis_power;
    Power_data.warning_power = Power_data.power_limit * 0.5f;
    Power_data.warning_power_buff = 40.0f;

    Power_data.now_power      = (float)referee->chassis_power;
    Power_data.now_power_buff = (float)referee->chassis_power_buffer;
    Power_data.total_current  = 0.0f;
}


/**
  * @brief      Power control
  * @param      chassis: Pointer to chassis motor group struct
  * @retval     NULL
  */
void Power_PowerControl(Motor_MotorGroupTypeDef* chassis) {

    if (Power_ControlMode != POWER_LIMIT) {
        return;
    }

    Power_SetLimitPower();

    if (Power_data.now_power_buff < Power_data.warning_power_buff) {
        float power_scale;
        if (Power_data.now_power_buff > 5.0f) {
            //scale down WARNING_POWER_BUFF
            power_scale = Power_data.now_power_buff / Power_data.warning_power_buff;
        }
        else {
            //only left 0% of WARNING_POWER_BUFF
            power_scale = 0.0f / Power_data.warning_power_buff;
        }   
        Power_data.total_current_limit = Power_data.buffer_total_current_limit * power_scale;
    }       //scale down
    else {
            //power > WARNING_POWER
        if(Power_data.now_power > Power_data.warning_power) {
            float power_scale;
                //power < limited
            if(Power_data.now_power < Power_data.power_limit) {
                //scale down
                power_scale = (Power_data.power_limit - Power_data.now_power) / (Power_data.power_limit - Power_data.warning_power);
            } 
            else {
                power_scale = 0.0f;
            }   //power > limited
            Power_data.total_current_limit = Power_data.buffer_total_current_limit + Power_data.power_total_current_limit * power_scale;
        }  
        else {
            Power_data.total_current_limit = Power_data.buffer_total_current_limit + Power_data.power_total_current_limit;
        }   //power < WARNING_POWER
    }
    //calculate the original motor current set
    for (uint8_t i = 0; i < 4; i++) {
        Power_data.total_current += fabs(chassis->motor_handle[i]->pid_spd.output);
    }
    if (Power_data.total_current > Power_data.total_current_limit) {
        float current_scale = Power_data.total_current_limit / Power_data.total_current;
        chassis->motor_handle[0]->pid_spd.output *= current_scale;
        chassis->motor_handle[1]->pid_spd.output *= current_scale;
        chassis->motor_handle[2]->pid_spd.output *= current_scale;
        chassis->motor_handle[3]->pid_spd.output *= current_scale;
    }
}


#endif
