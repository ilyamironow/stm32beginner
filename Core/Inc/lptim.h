/**
  ******************************************************************************
  * @file    lptim.h
  * @brief   This file contains all the function prototypes for
  *          the lptim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPTIM_H__
#define __LPTIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_LPTIM1_Init(void);
void MX_LPTIM2_Init(void);

/* USER CODE BEGIN Prototypes */
void startLPTIM1Counter(void);

void startPWMLPTIM2Counter(void);

void setCompareAutoReload(uint32_t cmp, uint32_t arr);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __LPTIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
