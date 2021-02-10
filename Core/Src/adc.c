/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  
}

/* USER CODE BEGIN 1 */
int32_t function(_Bool temp_volt) 
{
  /* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  int32_t vdda_appli;           
  
  /* Variable containing ADC conversions results */
  uint16_t aADCxConvertedData;
  int32_t uhADCxConvertedData_Vbat_mVolt = 0;            /* Value of internal voltage reference VrefInt calculated from ADC conversion data (unit: mV) */
  int32_t hADCxConvertedData_Temperature_DegreeCelsius = 0; /* Value of temperature calculated from ADC conversion data (unit: degree Celcius) */
  
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  
  /* Peripheral clock enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);
  
  /*    set active ADC1 configuration to default    */
  LL_ADC_DeInit(ADC1);
  
  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
  
  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(ADC1);
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(ADC1);
  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us. */
  /* Note: Variable divided by 2 to compensate partially */
  /* CPU processing cycles (depends on compilation optimization). */
  /* Note: If system core clock frequency is below 200kHz, wait time */
  /* is only a few CPU processing cycles. */
  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV32;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_12CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SINGLE_ENDED);
  
  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
  while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
      }
    }
#endif /* USE_TIMEOUT */
  }
  
  LL_ADC_Enable(ADC1);
  while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
      }
    }
#endif /* USE_TIMEOUT */
  }
  
  LL_ADC_REG_StartConversion(ADC1); //for stabilization and converting accurately
  while (LL_ADC_REG_IsConversionOngoing(ADC1)) {}
  while (LL_ADC_REG_IsStopConversionOngoing(ADC1)) {}
  
  LL_ADC_REG_StartConversion(ADC1);
  while (LL_ADC_REG_IsConversionOngoing(ADC1)) {}
  while (LL_ADC_REG_IsStopConversionOngoing(ADC1)) {}
  aADCxConvertedData = LL_ADC_REG_ReadConversionData12(ADC1);
  vdda_appli = __LL_ADC_CALC_VREFANALOG_VOLTAGE(aADCxConvertedData, LL_ADC_RESOLUTION_12B);
  
  /** Configure Regular Channel
  */
  if (temp_volt) 
  {
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VBAT);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VBAT);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VBAT, LL_ADC_SAMPLINGTIME_24CYCLES_5);
    LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_VBAT, LL_ADC_SINGLE_ENDED);
  }
  else 
  {
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_TEMPSENSOR);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_47CYCLES_5);
    LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SINGLE_ENDED);
  }
    
  LL_ADC_REG_StartConversion(ADC1); //for stabilization and converting accurately
  while (LL_ADC_REG_IsConversionOngoing(ADC1)) {}
  while (LL_ADC_REG_IsStopConversionOngoing(ADC1)) {}
  
  LL_ADC_REG_StartConversion(ADC1);
  while (LL_ADC_REG_IsConversionOngoing(ADC1)) {}
  while (LL_ADC_REG_IsStopConversionOngoing(ADC1)) {}
  aADCxConvertedData = LL_ADC_REG_ReadConversionData12(ADC1);
  if (temp_volt)
    uhADCxConvertedData_Vbat_mVolt = 3*__LL_ADC_CALC_DATA_TO_VOLTAGE(vdda_appli, aADCxConvertedData, LL_ADC_RESOLUTION_12B);
  else
    hADCxConvertedData_Temperature_DegreeCelsius = __LL_ADC_CALC_TEMPERATURE(vdda_appli, aADCxConvertedData, LL_ADC_RESOLUTION_12B);
  
  /*    set active ADC1 configuration to default    */
  LL_ADC_DeInit(ADC1);
  
  return (temp_volt) ?  uhADCxConvertedData_Vbat_mVolt:hADCxConvertedData_Temperature_DegreeCelsius;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
