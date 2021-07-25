 /*
 *  Project      : Infantry_Neptune
 * 
 *  file         : gim_miniPC_ctrl.c
 *  Description  : This file contains MiniPC control function
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-05-04 20:53:31
 *  LastEditTime : 2021-07-22 13:16:13
 */

#include "gim_miniPC_ctrl.h"

#if __FN_IF_ENABLE(__FN_CTRL_MINIPC)

#include "buscomm_ctrl.h"
#include "const.h"
#include "gim_gimbal_ctrl.h"
#include "gim_ins_ctrl.h"
#include "math_alg.h"
#include "key_periph.h"
#include "cmsis_os.h"

#define MINI_PC_TASK_PERIOD 1
#define dt MINI_PC_TASK_PERIOD         
MiniPC_MiniPCControlTypeDef MiniPC_MiniPCContrlData;

float autoaim_pitch_offset = -3.0f;
float autoaim_yaw_offset = 0.0f;

float energy_yaw_offset = 0.9f;
float energy_pitch_offset = 0.3f;

/**
  * @brief          MiniPC task
  * @param          NULL
  * @retval         NULL
  */
void MiniPC_Task(void const * argument) {

    for(;;) {
        while (!GLOBAL_INIT_FLAG) {
            osDelay(1);
        }
        MiniPC_UpdateControlData();
//        MiniPC_CalcAutoAim();
        MiniPC_SendHeartPacket();
      osDelay(MINI_PC_TASK_PERIOD);
    }
}


/**
  * @brief      Gets the pointer to the MiniPC data object
  * @param      NULL
  * @retval     Pointer to MiniPC data object
  */
MiniPC_MiniPCControlTypeDef* MiniPC_GetMiniPCControlDataPtr() {
    return &MiniPC_MiniPCContrlData;
}


/**
* @brief      Init minipc data
* @param      NULL
* @retval     NULL
*/
void MiniPC_ControlInit() {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();

    minipc->enable_aim_output = 1;
	minipc->enable_prediction = 1;

    static float Q_YAW_Init[4] = {
    0.5 * dt * dt * dt, 0.5 * dt * dt, 
    dt * dt,            dt, 
    };

    static float Q_PITCH_Init[4] = {
    0.5 * dt * dt * dt, 0.5 * dt * dt, 
    dt * dt,            dt, 
    };

    static float mat_R_YAW_diagonal_elements[2] = {0.5, 0.5};
    static float mat_R_PITCH_diagonal_elements[2] = {0.5, 0.5};

    static float P_Init[4] = {
        1, 0,
        0, 1
    };
    static float F_Init[4] = {
        1, dt, 
        0, 1
    };

    minipc->kf_pitch.UseAutoAdjustment = 1;
    minipc->kf_yaw.UseAutoAdjustment = 1;

    static uint8_t measurement_reference[2] = {1, 1}; 
    static float measurement_degree[2] = {1, 1};   
    static float state_min_variance[2] = {0.03, 0.005};

    Kalman_FilterInit(&minipc->kf_pitch, 2, 0, 2);
    Kalman_FilterInit(&minipc->kf_yaw, 2, 0, 2);

	//CVKF for Yaw Angle:
    memcpy(&minipc->kf_yaw.P_data, P_Init, sizeof(P_Init));
    memcpy(&minipc->kf_yaw.F_data, F_Init, sizeof(F_Init));
    memcpy(&minipc->kf_yaw.Q_data, Q_YAW_Init, sizeof(Q_YAW_Init));
    memcpy(&minipc->kf_yaw.MeasurementMap, measurement_reference, sizeof(measurement_reference));
    memcpy(&minipc->kf_yaw.MeasurementDegree, measurement_degree, sizeof(measurement_degree));
    memcpy(&minipc->kf_pitch.MatR_DiagonalElements, mat_R_YAW_diagonal_elements, sizeof(mat_R_YAW_diagonal_elements));
    memcpy(&minipc->kf_pitch.StateMinVariance, state_min_variance, sizeof(state_min_variance));

	//CVKF for Pitch Angle:
    memcpy(&minipc->kf_pitch.P_data, P_Init, sizeof(P_Init));
    memcpy(&minipc->kf_pitch.F_data, F_Init, sizeof(F_Init));
    memcpy(&minipc->kf_pitch.Q_data, Q_PITCH_Init, sizeof(Q_PITCH_Init));
    memcpy(&minipc->kf_pitch.MeasurementMap, measurement_reference, sizeof(measurement_reference));
    memcpy(&minipc->kf_pitch.MeasurementDegree, measurement_degree, sizeof(measurement_degree));
    memcpy(&minipc->kf_pitch.MatR_DiagonalElements, mat_R_PITCH_diagonal_elements, sizeof(mat_R_PITCH_diagonal_elements));
    memcpy(&minipc->kf_pitch.StateMinVariance, state_min_variance, sizeof(state_min_variance));
    
}


/**
* @brief      Change aiming mode
* @param      mode: MiniPC aim mode enum
* @retval     NULL
*/
void MiniPC_ChangeAimMode(MiniPC_AutoAimModeEnum mode) {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef *minipc_data = MiniPC_GetMiniPCDataPtr();
  
    minipc->aim_mode = mode;
    minipc_data->mode = minipc->aim_mode;
}


/**
* @brief      MiniPC auto aim decode control
* @param      NULL
* @retval     NULL
*/
void MiniPC_CalcAutoAim() {
	MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();
    
    // Set following mode
	MiniPC_SetTargetFollowMode();
    
	// Gimbal Trace Forecast
	if(minipc->enable_prediction == 1) {
        Kalman_FilterUpdate(&minipc->kf_yaw);
        Kalman_FilterUpdate(&minipc->kf_pitch);
    }

	MiniPC_SetGimbalRef();
}


/**
* @brief      Set gimbal following mode
* @param      mode: MiniPC target follow mode enum
* @retval     NULL
*/
void MiniPC_SetFollowMode(MiniPC_TargetFollowModeEnum mode) {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();

    minipc->target_state = mode;
}


/**
* @brief      Set the state of the target being recognized 
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetTargetFollowMode() {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();

    uint32_t now = HAL_GetTick();
    if (abs(now - minipc->get_target_time) <= Const_MiniPC_Follow_Target_Time) {
        MiniPC_SetFollowMode(MiniPC_TARGET_FOLLOWING);
    }
    else if (abs(now - minipc->get_target_time) >= Const_MiniPC_Lost_Target_Time) {
        MiniPC_SetFollowMode(MiniPC_TARGET_LOST);
    }
}


/**
* @brief      Add pitch axis trajectory compensation
* @param      angle_pitch: Set pitch axis angle
* @retval     float: Pitch axis angle after compensation
*/
float MiniPC_AddPitchOffset(float angle_pitch) {
	float offset = 0.0f; 
	if (angle_pitch > 0){
		offset = 10.0f;
	}
    else if (angle_pitch > -10.0f) {
		offset = 5.0f;
	}
    else {
		offset = 0.1f;
	}
	return offset;
}


/**
* @brief      Update minipc data
* @param      NULL
* @retval     NULL
*/
void MiniPC_UpdateControlData() {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef *minipc_data = MiniPC_GetMiniPCDataPtr();
	INS_IMUDataTypeDef *imu = Ins_GetIMUDataPtr();
    
    if (minipc_data->new_data_flag != 1) return;

    if (minipc_data->is_get_target == 1) {
        minipc->get_target_time = HAL_GetTick();
		// Update data when get target
		// Otherwise keep last data
		minipc->yaw_angle = minipc_data->yaw_angle;
		minipc->pitch_angle = minipc_data->pitch_angle;
        minipc->distance = minipc_data->distance;
    }

	//Update cvkf-mode when get new measurements!
	minipc->kf_yaw.MeasuredVector[0] = imu->angle.yaw - minipc->yaw_angle;
    minipc->kf_pitch.MeasuredVector[0] = imu->angle.pitch + minipc->pitch_angle;

    minipc->kf_yaw.MeasuredVector[1] = MiniPC_CV_CalInitSpeed(-minipc->yaw_angle);
    minipc->kf_pitch.MeasuredVector[1] = MiniPC_CV_CalInitSpeed(minipc->pitch_angle);
    minipc_data->new_data_flag = 0;
}


/**
* @brief      Set CV mode speed
* @param      NULL
* @retval     NULL
*/
float MiniPC_CV_CalInitSpeed(float delta_err_angle) {
	static float delta_angle = 0.0f;
	if (fabs(delta_err_angle) <= delta_angle) {
		return 0.0f;
	} else if(delta_err_angle > delta_angle) {
		return 10.0f;
	} else if(delta_err_angle < -delta_angle) {
		return -10.0f;
	}
	return 0.0f;
}


/**
* @brief      Set gimbal autoaim reference
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetAutoAimRef() {
	MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef *minipc_data = MiniPC_GetMiniPCDataPtr();
	INS_IMUDataTypeDef *imu = Ins_GetIMUDataPtr();
    Gimbal_GimbalTypeDef *gimbal = Gimbal_GetGimbalControlPtr();


	float cvkf_yaw_angle = 0.0f;
	float cvkf_pitch_angle = 0.0f;
	
    if (minipc->enable_prediction == 1) {
        cvkf_yaw_angle = minipc->kf_yaw.FilteredValue[0];
        cvkf_pitch_angle = minipc->kf_pitch.FilteredValue[1];
    }
	else {
		cvkf_yaw_angle   = minipc->yaw_angle;
		cvkf_pitch_angle = minipc->pitch_angle;
	}
	
	Gimbal_SetYawAutoRef(cvkf_yaw_angle + autoaim_yaw_offset);
	Gimbal_SetPitchAutoRef(cvkf_pitch_angle + autoaim_pitch_offset);
}


/**
* @brief      Set gimbal reference
* @param      NULL
* @retval     NULL
*/
void MiniPC_SetGimbalRef() {
    MiniPC_MiniPCControlTypeDef *minipc = MiniPC_GetMiniPCControlDataPtr();
    MiniPC_MiniPCDataTypeDef *minipc_data = MiniPC_GetMiniPCDataPtr();
	INS_IMUDataTypeDef *imu = Ins_GetIMUDataPtr();
    Gimbal_GimbalTypeDef *gimbal = Gimbal_GetGimbalControlPtr();
    
			
    if ((minipc->enable_aim_output) 
		 && (minipc->target_state ==  MiniPC_TARGET_FOLLOWING) 
		 && (gimbal->mode.present_mode == Gimbal_ARMOR)) {
	    
        MiniPC_SetAutoAimRef();
	}
		
    else if ((minipc->enable_aim_output) 
			 && (minipc->target_state ==  MiniPC_TARGET_FOLLOWING) 
			 && (gimbal->mode.present_mode == Gimbal_BIG_ENERGY)) {			
						
	    Gimbal_SetYawAutoRef(imu->angle.yaw - minipc->yaw_ref_filtered + energy_yaw_offset);
	    Gimbal_SetPitchAutoRef(imu->angle.pitch + minipc->pitch_ref_filtered + energy_pitch_offset);
	}   
	else if ((minipc->enable_aim_output) 
			 && (minipc->target_state ==  MiniPC_TARGET_FOLLOWING) 
			 && (gimbal->mode.present_mode == Gimbal_SMALL_ENERGY)) {
						
		Gimbal_SetYawAutoRef(imu->angle.yaw - minipc->yaw_ref_filtered + energy_yaw_offset);
	    Gimbal_SetPitchAutoRef(imu->angle.pitch + minipc->pitch_ref_filtered + energy_pitch_offset);
    }

    else return;

}

#endif
