/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : adc_util.c
 *  Description  : This file contains the ADC functions (Only for Super Cap)
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-06-13 12:17:44
 *  LastEditTime : 2021-07-09 07:37:10
 */

#include "adc_util.h"

#if __FN_IF_ENABLE(__FN_UTIL_ADC)

#include "const.h"

uint32_t Adc_valueBuf[3];  //Adc data array
float Adc_decodeBuf[3];    //Adc decode data

/**
  * @brief      Adc peripheral initialization
  * @param      NULL
  * @retval     NULL
  */
void Adc_Init() {
    //	HAL_ADCEx_Calibration_Start(&hadc1);						//Adc calibration
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&Adc_valueBuf, 3);  //start Adc DMA,Get the first group data.
}

/**
  * @brief      Get Adc data
  * @param      hadc1 : adc handle
  * @param      ADC_valueBuf : adc_value array
  * @retval     NULL
  */
void Adc_GetData() {
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&Adc_valueBuf, 3);
}

/**
  * @brief      Decode Adc data
  * @param      NULL
  * @retval     NULL
  */
void Adc_Decode() {
    memset(Adc_decodeBuf, 0, sizeof(Adc_decodeBuf));
    for (int j = 0; j <= 3; j++)
        Adc_decodeBuf[j] = (float)Adc_valueBuf[j] / 4096.0f * 3.3f;  //adc decode 3.3V AVCC
}

#endif
