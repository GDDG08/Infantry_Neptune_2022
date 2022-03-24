/*
 * @Project      : RM_Infantry_Neptune
 * @FilePath     : \infantry_-neptune\Core\Src\Utility\can_util.c
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2021-12-31 17:37:14
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-03-24 20:02:48
 */

#include "can_util.h"

#if __FN_IF_ENABLE(__FN_UTIL_CAN)

#include "motor_periph.h"
#include "buscomm_ctrl.h"

CAN_RxHeaderTypeDef Can_RxHeader;
const uint16_t Const_Can_RX_BUFF_LEN = 200;
uint8_t Can_RxData[Const_Can_RX_BUFF_LEN];

/**
 * @brief      CAN Error handle handling
 * @param      ret: error data
 * @retval     NULL
 */
void Can_ErrorHandler(uint32_t ret) {
    // Log_DebugPrintf("Error: CAN Error!\n");
    while (1) {
        return;
    }
}

/**
 * @brief      Initialize can transmitter
 * @param      pheader: Pointer to the initialized header
 * @param      stdid: CAN Equipment number
 * @retval     NULL
 */
void Can_InitTxHeader(CAN_TxHeaderTypeDef* pheader, uint32_t stdid, uint32_t extid, uint32_t dlc) {
    pheader->StdId = stdid;
    pheader->ExtId = extid;
    pheader->RTR = CAN_RTR_DATA;
    pheader->IDE = CAN_ID_STD;
    pheader->DLC = dlc;
    pheader->TransmitGlobalTime = DISABLE;
}

/**
 * @brief      Initialize can filter and enable CAN Bus Transceiver
 * @param      phcan: Pointer to the CAN header
 * @retval     NULL
 */
void Can_InitFilterAndStart(CAN_HandleTypeDef* phcan) {
    CAN_FilterTypeDef sFilterConfig;

    if (phcan == &hcan1)
        sFilterConfig.FilterBank = 0;
    else
        sFilterConfig.FilterBank = 14;

    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    uint32_t ret = HAL_CAN_ConfigFilter(phcan, &sFilterConfig);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }

    ret = HAL_CAN_Start(phcan);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }

    ret = HAL_CAN_ActivateNotification(phcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }
}

/**
 * @brief      Sending information to can bus
 * @param      phcan: Pointer to the CAN header
 * @param      pheader: Pointer to the CAN tx header
 * @param      txdata: Message to send
 * @retval     NULL
 */
void Can_SendMessage(CAN_HandleTypeDef* phcan, CAN_TxHeaderTypeDef* pheader, uint8_t txdata[]) {
    uint32_t mailbox;
    /* Start the Transmission process */
    uint32_t ret = HAL_CAN_AddTxMessage(phcan, pheader, txdata, &mailbox);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Can_ErrorHandler(ret);
    }
}

/**
 * @brief      HAL_CAN_ Rx Fifo0 Message Pending Call back
 * @param      phcan: Pointer to the CAN header
 * @retval     NULL
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* phcan) {
    /* Get RX message */
    uint32_t ret = HAL_CAN_GetRxMessage(phcan, CAN_RX_FIFO0, &Can_RxHeader, Can_RxData);
    if (ret != HAL_OK) {
        /* Reception Error */
        Can_ErrorHandler(ret);
    }
    Can_RxMessageCallback(phcan, &Can_RxHeader, Can_RxData);
}

/**
 * @brief      Can bus data receiving callback function that updates the motor status according to the received information
 * @param      phcan: Pointer to the CAN header
 * @param      rxheader: Pointer to the CAN receive header
 * @param      rxdata: The message CAN receive
 * @retval     NULL
 */
void Can_RxMessageCallback(CAN_HandleTypeDef* phcan, CAN_RxHeaderTypeDef* rxheader, uint8_t rxdata[]) {
    BusComm_CANRxCallback(phcan, rxheader->StdId, rxdata, rxheader->DLC);

#if __FN_IF_ENABLE(__FN_PERIPH_MOTOR)
    Motor_EncoderDecodeCallback(phcan, rxheader->StdId, rxdata, rxheader->DLC);  //
#endif
}

#endif
