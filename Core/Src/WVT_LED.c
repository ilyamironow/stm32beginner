/*!
 * \file WVT_LED.c
 * \brief LED mode functions
 * \version 0.1
 * \date 03-03-2021
 * \copyright WAVIoT 2021
 */

/* Includes ------------------------------------------------------------------*/
#include "lptim.h"
/* USER CODE BEGIN 0 */
#include "WVT_LED.h"

uint8_t repetition = 1; // Number of LED glows required in the current LED mode
uint8_t cycles = 1;
uint8_t flag = 1;
extern enum mode CurMode;

/*!
* \brief Lights LED one time according to current LED mode
*
* \param [IN] enum mode value
* \retval void
*/
void LEDModeExecution(enum mode selected_mode) 
{
  if (flag == 1)
  {
    flag = 0;
    uint32_t startTime = 0, endTime = 1000;
    startLPTIM2Counter();
    
    /*  set variables for current LED mode  */
    switch (selected_mode)
    {
    case THREE_SHORT:
      repetition = 3;
      startTime = 500;
      break;
      
    case LONG_AND_TWO_SHORT:
      repetition = 3;
      startTime = 300;
      endTime = 2000;
      if (cycles > 1)
      {
        startTime = 500;
        endTime = 1000;
      }
      break;
      
    case TWO_VERY_SHORT:
      repetition = 2;
      startTime = 250;
      endTime = 500;
      break;
      
    case FIVE_SHORT:
      repetition = 5;
      startTime = 500;
      break;  
      
    case TWO_LONG:
      repetition = 2;
      startTime = 300;
      endTime = 2000;
      break;
      
    case THREE_VERY_SHORT_AND_LONG:
      repetition = 4;
      startTime = 250;
      endTime = 500;
      if (cycles > 3)
      {
        startTime = 300;
        endTime = 2000;
      }
      break;
      
    case SHORT_AND_VERY_SHORT_SHORT_AND_VERY_SHORT:
      repetition = 4;
      startTime = 500;
      endTime = 1000;
      if (cycles == 2 || cycles == 4)
      {
        startTime = 250;
        endTime = 500;
      }
      break;
      
    case FOUR_VERY_SHORT:
      repetition = 4;
      startTime = 250;
      endTime = 500;
      break;
      
    case SHORT_AND_LONG:
      repetition = 2;
      startTime = 500;
      if (cycles > 1)
      {
        startTime = 300;
        endTime = 2000;
      }
      break;
      
    case LONG_AND_TWO_VERY_SHORT_AND_LONG:
      repetition = 4;
      startTime = 300;
      endTime = 2000;
      if (cycles > 1 && cycles < 4)
      {
        startTime = 250;
        endTime = 500;
      }
      break;
    }
    
    /*  Lights LED for required time interval  */
    setCompareAutoReload(startTime, endTime);
  }
  /* To make sure that __WFI() executes and not LEDModeExecution again */
  if (flag == 2)
    flag = 1;
}

/*!
* \brief Continues the animation or chooses the next LED mode
* @note  Continues the animation when LED was not lighted required number of cycles
* @note  or the animation has ended and next mode should be chosen. In the last
* @note  case LPTIM1 starts. 
* \retval void
*/
void LEDModeContinuation(void)
{
  if (cycles != repetition)
  {
    cycles = cycles + 1;
    flag = 1;
    LEDModeExecution(CurMode);
  }
  else 
  {
    cycles = 1;
    flag = 2;
    CurMode = (enum mode) ((CurMode + 1) % LED_MODES_NUMBER); // next mode
    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);
    /* Enter STOP 2 mode */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep(); 
  }
}
/* USER CODE END 0 */