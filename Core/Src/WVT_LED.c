/* Includes ------------------------------------------------------------------*/
#include "lptim.h"
/* USER CODE BEGIN 0 */
#include "WVT_LED.h"

uint8_t repetition = 1;
uint8_t cycles = 1;
uint8_t flag = 1;
extern enum mode CurMode;

void LEDModeExecution(enum mode selected_mode) 
{
  if (flag == 1)
  {
    flag = 0;
    uint32_t startTime = 0, endTime = 1000;
    startLPTIM2Counter();
    switch (selected_mode)
    {
    case THREE_SHORT:
      repetition = 3;
      startTime = 500;
      break;
      
    case LONG_AND_TWO_SHORT:
      startTime = 300;
      endTime = 2000;
      if (cycles > 1)
      {
        repetition = 3;
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
    }
    setCompareAutoReload(startTime, endTime);
  }
  //so that __WFI() executes and not LEDModeExecution
  if (flag == 2)
    flag = 1;
}

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
    CurMode = (enum mode) ((CurMode + 1) % LED_MODES_NUMBER);
    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);
    /* Enter STOP 2 mode */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep(); 
  }
}
/* USER CODE END 0 */