/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : dac_util.c
 *  Description  : This file contains the DAC functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-11 14:19:14
 *  LastEditTime : 2021-07-11 08:34:11
 */


#include "dac_util.h"

#if __FN_IF_ENABLE(__FN_UTIL_DAC)

#include "const.h"

Dac_DacHandleTypeDef CurrentDac;
DAC_HandleTypeDef *Current_Dac_HANDLER = &hdac;
    

/**
  * @brief      DAC initialization
  * @param      NULL
  * @retval     NULL
  */
void Dac_Init() {
    //Initialization related parameters
    CurrentDac.state = DAC_OFF;
    CurrentDac.ch = DAC_CHANNEL_1;
    CurrentDac.Dac_DecodeValue = 0;
    CurrentDac.hdac = Current_Dac_HANDLER;
    CurrentDac.value = 0;
    
    //Close DAC output after initialization
    HAL_DAC_Stop(CurrentDac.hdac, CurrentDac.ch);                      
}
        


/**
  * @brief      Turn on DAC and DMA
  * @param      value :Set current value(unit: A)
  * @retval     NULL
  */
void Dac_SetCurrent(float value) {

    //decoding
    CurrentDac.value = value;
    Dac_DecodeValue();
    
    //Set dma and dac
    HAL_DAC_SetValue(CurrentDac.hdac, CurrentDac.ch , DAC_ALIGN_12B_R , CurrentDac.Dac_DecodeValue);
    HAL_DAC_Start(CurrentDac.hdac, CurrentDac.ch);
    CurrentDac.state = DAC_ON;
}


/**
  * @brief      Close DAC  
  * @param      NULL
  * @retval     NULL
  */
void Dac_StopDAC() {
    HAL_DAC_Stop(CurrentDac.hdac, CurrentDac.ch);                      
    HAL_DAC_Stop_DMA(CurrentDac.hdac, CurrentDac.ch);
    CurrentDac.state = DAC_OFF;
}


/**
 * @brief      Calculate DAC set value
 * @param      NULL
 * @retval     NULL
 */
void Dac_DecodeValue() {
    float voltage = CurrentDac.value * Const_DAC_DetectRES * Const_DAC_GAIN * 4.3f;
		if (voltage >= 3.1f) {
			voltage = 3.1f;
		}
    float decode  = voltage * 4096 / 3.3f;
    CurrentDac.Dac_DecodeValue = decode;
}

#endif
