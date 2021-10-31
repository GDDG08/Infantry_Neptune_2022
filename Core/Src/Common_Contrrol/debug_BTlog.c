/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \infantry_-neptune\Core\Src\Common_Contrrol\debug_BTlog.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-31 09:16:32
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-31 09:49:45
 */

#include "debug_BTlog.h"

#if __FN_IF_ENABLE(__FN_DEBUG_BTLOG)

#if __FN_IF_ENABLE(__FN_INFANTRY_GIMBAL)
UART_HandleTypeDef* Const_BTlog_UART_HANDLER = &huart6;
#elif __FN_IF_ENABLE(__FN_INFANTRY_CHASSIS)
UART_HandleTypeDef* Const_BTlog_UART_HANDLER = &huart5;

/*              Debug BTlog constant            */
const uint32_t Const_MiniPC_HEART_SENT_PERIOD = 100;  // (ms)

const uint16_t Const_MiniPC_TX_BUFF_LEN = 200;
const uint16_t Const_MiniPC_MINIPC_OFFLINE_TIME = 1000;
const uint16_t Const_MiniPC_TX_HEART_FRAME_LEN = 10;  // miniPC heart transmit frame length

const uint8_t Const_MiniPC_SLAVE_COMPUTER = 0x00;
const uint8_t Const_MiniPC_INFANTRY_3 = 0x03;
const uint8_t Const_MiniPC_INFANTRY_4 = 0x04;
const uint8_t Const_MiniPC_INFANTRY_5 = 0x05;

const uint8_t Const_MiniPC_COLOR_RED = 0x00;
const uint8_t Const_MiniPC_COLOR_BLUE = 0x01;

const uint8_t Const_MiniPC_ARMOR = 0x00;
const uint8_t Const_MiniPC_BIG_BUFF = 0x01;
const uint8_t Const_MiniPC_LITTLE_BUFF = 0x02;

const uint8_t Const_MiniPC_PACKET_HEADR_0 = 0xa5;
const uint8_t Const_MiniPC_PACKET_HEADR_1 = 0X5a;

const uint8_t Const_MiniPC_Heart_PACKET = 0x00;
const uint8_t Const_MiniPC_ARMOR_PACKET = 0x02;

const uint8_t Const_MiniPC_Heart_PACKET_DATA_LEN = 2;

uint8_t MiniPC_TxData[Const_MiniPC_TX_BUFF_LEN];

void BTlog_Init() {
}

void BTlog_Send() {
    static uint32_t heart_count = 0;
    if ((HAL_GetTick() - heart_count) <= Const_MiniPC_HEART_SENT_PERIOD)
        return;

    MiniPC_Update();

    MiniPC_MiniPCDataTypeDef* minipc = MiniPC_GetMiniPCDataPtr();

    minipc->state = MiniPC_PENDING;

    uint8_t* buff = MiniPC_TxData;
    int size = Const_MiniPC_TX_HEART_FRAME_LEN;
    buff[0] = Const_MiniPC_PACKET_HEADR_0;
    buff[1] = Const_MiniPC_PACKET_HEADR_1;
    buff[2] = Const_MiniPC_SLAVE_COMPUTER;
    buff[3] = minipc->addressee;
    buff[4] = Const_MiniPC_Heart_PACKET;
    buff[5] = Const_MiniPC_Heart_PACKET_DATA_LEN;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = minipc->team_color;
    buff[9] = minipc->mode;

    heart_count = HAL_GetTick();

    if (HAL_UART_GetState(Const_MiniPC_UART_HANDLER) & 0x01)
        return;  // tx busy
    Uart_SendMessage_IT(Const_MiniPC_UART_HANDLER, buff, size);
}

#endif
