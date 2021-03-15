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
 * \brief time intervals(from start to end) for VERY_SHORT|SHORT|LONG in milliseconds
 *
 */
#define START_VERY_SHORT 250
#define END_VERY_SHORT 500
#define START_SHORT 500
#define END_SHORT 1000
#define START_LONG 300
#define END_LONG 2000

/* Number of LED glows required in the current LED mode */
uint8_t Repetition = 1; 
/* Current amount of LED glows */
uint8_t CurrentCycle = 1; 
/* LED mode that will be executed */
enum mode SelectedMode;

/*!
* \brief Lights LED one time according to current LED mode
*
* \param [IN] enum mode value
* \retval void
*/
void LEDModeExecution(enum mode SelectedMode)  
{
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
    if (CurrentCycle > 1)
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
    if (CurrentCycle > 3)
    {
      startTime = START_LONG;
      endTime = END_LONG;
    }
    break;
    
  case SHORT_AND_VERY_SHORT_TWICE:
    Repetition = 4;
    startTime = END_VERY_SHORT;
    endTime = END_SHORT;
    if (CurrentCycle == 2 || CurrentCycle == 4)
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
    if (CurrentCycle > 1)
    {
      startTime = START_LONG;
      endTime = END_LONG;
    }
    break;
    
  case LONG_AND_TWO_VERY_SHORT_AND_LONG:
    Repetition = 4;
    startTime = START_LONG;
    endTime = END_LONG;
    if (CurrentCycle > 1 && CurrentCycle < 4)
    {
      startTime = START_VERY_SHORT;
      endTime = END_VERY_SHORT;
    }
    break;
    
  default:
    /* possible action */
    break;
  }
  /*  Lights LED for required time interval  */
  setCompareAutoReload(startTime, endTime);
}

/*!
* \brief Continues the animation or chooses the next LED mode
* \note  Continues the animation when LED was not lighted required number of cycles
* \note  or the animation has ended and next mode should be chosen. In the last
* \note  case LPTIM1 starts. 
* \retval void
*/
void LEDModeContinuation(void)
{
  if (CurrentCycle != Repetition)
  {
    CurrentCycle = CurrentCycle + 1;
    LEDModeExecution(SelectedMode);
  }
  else 
    CurrentCycle = 1;
}
/* USER CODE END 0 */
