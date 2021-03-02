/* Includes ------------------------------------------------------------------*/
#include "lptim.h"
/* USER CODE BEGIN 0 */
uint8_t repetition = 1;

extern uint8_t cycles;
_Bool flag, flag2;

void LED_mode_execution(enum mode selected_mode) 
{
  if (flag)
  {
    flag = 0;
    uint32_t startTime = 0, endTime = 1000;
    Start_LPTIM2_Counter();
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
    Set_values_REP_CMP_ARR(startTime, endTime);
  } else if (flag2)
  {
    flag2 = 0;
    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);
    /* Enter STOP 2 mode */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep();  
    /* Request Wait For Interrupt */
    __WFI();
  }
}
/* USER CODE END 0 */