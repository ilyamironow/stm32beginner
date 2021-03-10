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

/*!
 * \brief time intervals for VERY_SHORT|SHORT|LONG in milliseconds
 *
 */
#define START_VERY_SHORT 250
#define END_VERY_SHORT 500
#define START_SHORT 500
#define END_SHORT 1000
#define START_LONG 300
#define END_LONG 2000

uint8_t Repetition = 1; // Number of LED glows required in the current LED mode
uint8_t Cycles = 1;
uint8_t CanExecute = 1; // 0 - not ready; 1 - ready; 2 -  pre-ready
enum mode SelectedMode = THREE_SHORT; // first LED mode that will be executed

/*!
* \brief Lights LED one time according to current LED mode
*
* \param [IN] enum mode value
* \retval void
*/
void LEDModeExecution() 
{
  if (CanExecute == 1)
  {
    CanExecute = 0;
    uint32_t startTime = 0, endTime = END_SHORT;
    startPWMLPTIM2Counter();
    
    /*  set variables for current LED mode  */
    switch (SelectedMode)
    {
    case THREE_SHORT:
      Repetition = 3;
      startTime = START_SHORT;
      break;
      
    case LONG_AND_TWO_SHORT:
      Repetition = 3;
      startTime = START_LONG;
      endTime = END_LONG;
      if (Cycles > 1)
      {
        startTime = START_SHORT;
        endTime = END_SHORT;
      }
      break;
      
    case TWO_VERY_SHORT:
      Repetition = 2;
      startTime = START_VERY_SHORT;
      endTime = END_VERY_SHORT;
      break;
      
    case FIVE_SHORT:
      Repetition = 5;
      startTime = START_SHORT;
      break;  
      
    case TWO_LONG:
      Repetition = 2;
      startTime = START_LONG;
      endTime = END_LONG;
      break;
      
    case THREE_VERY_SHORT_AND_LONG:
      Repetition = 4;
      startTime = START_VERY_SHORT;
      endTime = END_VERY_SHORT;
      if (Cycles > 3)
      {
        startTime = START_LONG;
        endTime = END_LONG;
      }
      break;
      
    case SHORT_AND_VERY_SHORT_SHORT_AND_VERY_SHORT:
      Repetition = 4;
      startTime = END_VERY_SHORT;
      endTime = END_SHORT;
      if (Cycles == 2 || Cycles == 4)
      {
        startTime = START_VERY_SHORT;
        endTime = END_VERY_SHORT;
      }
      break;
      
    case FOUR_VERY_SHORT:
      Repetition = 4;
      startTime = START_VERY_SHORT;
      endTime = END_VERY_SHORT;
      break;
      
    case SHORT_AND_LONG:
      Repetition = 2;
      startTime = START_SHORT;
      if (Cycles > 1)
      {
        startTime = START_LONG;
        endTime = END_LONG;
      }
      break;
      
    case LONG_AND_TWO_VERY_SHORT_AND_LONG:
      Repetition = 4;
      startTime = START_LONG;
      endTime = END_LONG;
      if (Cycles > 1 && Cycles < 4)
      {
        startTime = START_VERY_SHORT;
        endTime = END_VERY_SHORT;
      }
      break;
    }
    
    /*  Lights LED for required time interval  */
    setCompareAutoReload(startTime, endTime);
  }
  /* To make sure that __WFI() executes and not LEDModeExecution again */
  if (CanExecute == 2)
    CanExecute = 1;
}

/*!
* \brief Continues the animation or chooses the next LED mode
* \note  Continues the animation when LED was not lighted required number of Cycles
* \note  or the animation has ended and next mode should be chosen. In the last
* \note  case LPTIM1 starts. 
* \retval void
*/
void LEDModeContinuation(void)
{
  if (Cycles != Repetition)
  {
    Cycles = Cycles + 1;
    CanExecute = 1;
    LEDModeExecution();
  }
  else 
  {
    Cycles = 1;
    CanExecute = 2;
    SelectedMode = (enum mode) ((SelectedMode + 1) % LED_MODES_NUMBER); // next mode
    startLPTIM1Counter();
  }
}
/* USER CODE END 0 */
