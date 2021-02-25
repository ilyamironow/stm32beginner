/* Includes ------------------------------------------------------------------*/
#include "lptim.h"
/* USER CODE BEGIN 0 */

#define second 32000

void LED_mode_execution(enum mode selected_mode) 
{
  switch (selected_mode)
  {
  case threetimesshort:
    Set_values_REP_CMP_ARR(3, second/2, second); 
    break;
  case longandtwoshort:
    Set_values_REP_CMP_ARR(1, 0, 2*second); 
    LL_LPTIM_Disable(LPTIM2);
    Start_LPTIM2_Counter();
    Set_values_REP_CMP_ARR(2, second/2, second); 
    break;
  }
}
/* USER CODE END 0 */