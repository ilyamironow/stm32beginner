/*!
 * \file WVT_LED.h
 * \brief LED mode functions
 * \version 0.1
 * \date 03-03-2021
 *
 * \copyright WAVIoT 2021
 *
 */

enum mode {
    THREE_SHORT,
    LONG_AND_TWO_SHORT,
    TWO_VERY_SHORT,
    FIVE_SHORT,
    TWO_LONG,
    LED_MODES_NUMBER
};

void LEDModeExecution(enum mode selected_mode);

void LEDModeContinuation(void);
