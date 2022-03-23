/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \infantry_-neptune\Core\Src\Common_Contrrol\buscomm_cmd.c
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2021-12-22 22:06:02
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-03-20 12:01:07
 */

#include "buscomm_cmd.h"
#include "buscomm_ctrl.h"
#include "const.h"

#if __FN_IF_ENABLE(__FN_INFANTRY_CHASSIS)
#include "cha_chassis_ctrl.h"
#include "cha_gimbal_ctrl.h"
#include "referee_periph.h"
#include "cha_power_ctrl.h"
#endif

#if __FN_IF_ENABLE(__FN_INFANTRY_GIMBAL)
#include "gim_gimbal_ctrl.h"
#endif

#if __FN_IF_ENABLE(__FN_SUPER_CAP)
#include "supercap_ctrl.h"
#endif

const uint32_t CMD_SET_YAW_ANGLE_BASIC_DATA = 0x205;
const uint32_t CMD_SET_17MM_DATA = 0x206;

const uint32_t CMD_SET_MODE = 0x201;
const uint32_t CMD_SET_GIMBAL_DATA = 0x202;
const uint32_t CMD_SET_IMU_YAW = 0x203;
const uint32_t CMD_SET_CHA_REF = 0x204;

const uint32_t CMD_SEND_CAP_STATE = 0x207;
const uint32_t CMD_SET_CAP_MODE = 0x98;
const uint32_t CMD_SET_CAP_STATE_1 = 0x299;
const uint32_t CMD_SET_CAP_STATE_2 = 0x298;

const uint32_t CMD_CHASSIS_SEND_PACK_1 = 0xA1;
const uint32_t CMD_CHASSIS_SEND_PACK_2 = 0xA2;
const uint32_t CMD_CHASSIS_SEND_PACK_3 = 0xA3;

const uint32_t CMD_GIMBAL_SEND_PACK_1 = 0xB1;
const uint32_t CMD_GIMBAL_SEND_PACK_2 = 0xB2;
const uint32_t CMD_GIMBAL_SEND_PACK_3 = 0xB3;
const uint32_t CMD_GIMBAL_SEND_PACK_4 = 0xB4;

const uint32_t CMD_SUPERCAP_SEND_PACK_1 = 0xC1;

static void _send_yaw_angle_basic_data(uint8_t buff[]);
static void _send_17mm_data(uint8_t buff[]);
static void _send_mode(uint8_t buff[]);
static void _send_gimbal_data(uint8_t buff[]);
static void _send_imu_yaw(uint8_t buff[]);
static void _send_chassis_ref(uint8_t buff[]);
static void _send_cap_state(uint8_t buff[]);
static void _send_cap_mode(uint8_t buff[]);
static void _set_yaw_angle_basic_data(uint8_t buff[]);
static void _set_17mm_data(uint8_t buff[]);
static void _set_mode(uint8_t buff[]);
static void _set_gimbal_data(uint8_t buff[]);
static void _set_imu_yaw(uint8_t buff[]);
static void _set_cha_ref(uint8_t buff[]);
static void _set_cap_state(uint8_t buff[]);
static void _set_cap_state_1(uint8_t buff[]);
static void _set_cap_state_2(uint8_t buff[]);

BusCmd_TableEntry Buscmd_Receive[10] = {
    {0xff, NULL},
    {CMD_SET_YAW_ANGLE_BASIC_DATA, &_set_yaw_angle_basic_data},
    {CMD_SET_17MM_DATA, &_set_17mm_data},
    {CMD_SET_MODE, &_set_mode},
    {CMD_SET_GIMBAL_DATA, &_set_gimbal_data},
    {CMD_SET_IMU_YAW, &_set_imu_yaw},
    {CMD_SET_CHA_REF, &_set_cha_ref},
    {CMD_SEND_CAP_STATE, &_set_cap_state},
    {CMD_SET_CAP_STATE_1, &_set_cap_state_1},
    {CMD_SET_CAP_STATE_2, &_set_cap_state_2}};

BusCmd_TableEntry Buscmd_GimSend[4] = {
    {CMD_GIMBAL_SEND_PACK_1, &_send_mode},
    {CMD_GIMBAL_SEND_PACK_2, &_send_gimbal_data},
    {CMD_GIMBAL_SEND_PACK_3, &_send_imu_yaw},
    {CMD_GIMBAL_SEND_PACK_4, &_send_chassis_ref}};

BusCmd_TableEntry Buscmd_ChaSend[3] = {
    {CMD_CHASSIS_SEND_PACK_1, &_send_yaw_angle_basic_data},
    {CMD_CHASSIS_SEND_PACK_2, &_send_17mm_data},
    {CMD_CHASSIS_SEND_PACK_3, &_send_cap_mode}};

BusCmd_TableEntry Buscmd_CapSend[1] = {
    {CMD_SUPERCAP_SEND_PACK_1, &_send_cap_state}};

int count1a, count2a, count3a, count4a, count5a, count6a, count7a, count8a;
float rate1a, rate2a, rate3a, rate4a, rate5a, rate6a, rate7a, rate8a;

/*      send functions driver       */
static void _send_yaw_angle_basic_data(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_ChaYawAngleBasicData;
    count1a++;
    rate1a = 1000 * count1a / HAL_GetTick();
    memset(buff, 0, 8);
    float2buff(buscomm->yaw_relative_angle, buff);
    buff[4] = buscomm->robot_id;
    buff[5] = buscomm->power_limit;
    buff[6] = buscomm->main_shooter_power;
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_17mm_data(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_Cha17mmData;
    count2a++;
    rate2a = 1000 * count2a / HAL_GetTick();
    memset(buff, 0, 8);
    float2buff(buscomm->speed_17mm, buff);
    ui162buff(buscomm->heat_cooling_limit, buff + 2);
    ui162buff(buscomm->heat_17mm, buff + 4);
    ui162buff(buscomm->heat_speed_limit, buff + 6);
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_mode(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_GimMode;
    count3a++;
    rate3a = 1000 * count3a / HAL_GetTick();
    memset(buff, 0, 8);
    buff[0] = buscomm->gimbal_yaw_mode;
    buff[1] = buscomm->power_limit_mode;
    buff[2] = buscomm->cap_boost_mode_user;
    buff[3] = buscomm->cap_mode_user;
    buff[4] = buscomm->chassis_mode;
    buff[5] = buscomm->ui_cmd;
    buff[6] = buscomm->infantry_code;
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_gimbal_data(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_GimGimbalData;
    count4a++;
    rate4a = 1000 * count4a / HAL_GetTick();
    memset(buff, 0, 8);
    float2buff(buscomm->pitch_angle, buff);
    float2buff(buscomm->gimbal_yaw_ref, buff + 4);
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_imu_yaw(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_GimImuYaw;
    count5a++;
    rate5a = 1000 * count5a / HAL_GetTick();
    memset(buff, 0, 8);
    float2buff(buscomm->gimbal_imu_spd, buff);
    float2buff(buscomm->gimbal_imu_pos, buff + 4);
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_chassis_ref(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_GimChassisRef;
    count6a++;
    rate6a = 1000 * count6a / HAL_GetTick();
    memset(buff, 0, 8);
    float2buff(buscomm->chassis_fb_ref, buff);
    float2buff(buscomm->chassis_lr_ref, buff + 4);
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_cap_state(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_CapState;
    count7a++;
    rate7a = 1000 * count7a / HAL_GetTick();
    memset(buff, 0, 8);
    buff[0] = buscomm->cap_state;
    buff[1] = buscomm->cap_rest_energy;
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

static void _send_cap_mode(uint8_t buff[]) {
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    CAN_TxHeaderTypeDef* pheader = &BusComm_CapMode;
    count8a++;
    rate8a = 1000 * count8a / HAL_GetTick();
    memset(buff, 0, 8);
    buff[0] = buscomm->cap_mode_fnl;
    buff[1] = buscomm->cap_boost_mode_fnl;
    buff[2] = buscomm->chassis_power_limit;
    buff[3] = buscomm->chassis_power_buffer;
    float2buff(buscomm->chassis_power, buff + 4);
    Can_SendMessage(Const_BusComm_CAN_HANDLER, pheader, buff);
}

int count1;
float rate1;
/*          function driver      */
static void _set_yaw_angle_basic_data(uint8_t buff[]) {
    count1++;
    rate1 = 1000 * count1 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    buscomm->yaw_relative_angle = buff2float(buff);
    buscomm->robot_id = buff[4];
    buscomm->power_limit = buff[5];
    buscomm->main_shooter_power = buff[6];
}
int count2;
float rate2;
static void _set_17mm_data(uint8_t buff[]) {
    count2++;
    rate2 = 1000 * count2 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    buscomm->speed_17mm = buff2float(buff);
    buscomm->heat_cooling_limit = buff2ui16(buff + 2);
    buscomm->heat_17mm = buff2ui16(buff + 4);
    buscomm->heat_speed_limit = buff2ui16(buff + 6);
}
int count3;
float rate3;
static void _set_mode(uint8_t buff[]) {
    count3++;
    rate3 = 1000 * count3 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    // Todo
    if ((buscomm->cap_mode_user == SUPERCAP_CTRL_ON) && (buff[3] == SUPERCAP_CTRL_OFF)) {
        buscomm->power_path_change_flag = HAL_GetTick();
    }

    buscomm->gimbal_yaw_mode = buff[0];
    buscomm->power_limit_mode = buff[1];
    buscomm->cap_boost_mode_user = buff[2];
    buscomm->cap_mode_user = buff[3];
    buscomm->chassis_mode = buff[4];
    buscomm->ui_cmd = buff[5];
    buscomm->infantry_code = buff[6];
    _cmd_mode_control();
}
int count4;
float rate4;
static void _set_gimbal_data(uint8_t buff[]) {
    count4++;
    rate4 = 1000 * count4 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    buscomm->pitch_angle = buff2float(buff);
    buscomm->gimbal_yaw_ref = buff2float(buff + 4);
}
int count5;
float rate5;
static void _set_imu_yaw(uint8_t buff[]) {
    count5++;
    rate5 = 1000 * count5 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    buscomm->gimbal_imu_spd = buff2float(buff);
    buscomm->gimbal_imu_pos = buff2float(buff + 4);
}
int count6;
float rate6;
static void _set_cha_ref(uint8_t buff[]) {
    count6++;
    rate6 = 1000 * count6 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();
    buscomm->chassis_fb_ref = buff2float(buff);
    buscomm->chassis_lr_ref = buff2float(buff + 4);
}
int count7;
float rate7;
static void _set_cap_state(uint8_t buff[]) {
    count7++;
    rate7 = 1000 * count7 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    buscomm->cap_state = buff[0];
    buscomm->cap_rest_energy = buff[1];
    buscomm->cap_rest_energy_display = (float)buscomm->cap_rest_energy;
}
int count8;
float rate8;
static void _set_cap_state_1(uint8_t buff[]) {
    count8++;
    rate8 = 1000 * count8 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    buscomm->Cap_power = buff2float(buff);
    buscomm->cap_rest_energy = buff[4];
}
int count9;
float rate9;
static void _set_cap_state_2(uint8_t buff[]) {
    count9++;
    rate9 = 1000 * count9 / HAL_GetTick();
    BusComm_BusCommDataTypeDef* buscomm = BusComm_GetBusDataPtr();

    buscomm->Cap_voltage = buff2float(buff);
    buscomm->Cap_current = buff2float(buff + 4);
}
