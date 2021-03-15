/*!
 * \file WVT_LED.h
 * \brief LED mode functions
 * \version 0.1
 * \date 03-03-2021
 * \copyright WAVIoT 2021
 */

/* glow modes of LED */
enum mode {
    THREE_SHORT,
    LONG_AND_TWO_SHORT,
    TWO_VERY_SHORT,
    FIVE_SHORT,
    TWO_LONG,
    THREE_VERY_SHORT_AND_LONG,
    SHORT_AND_VERY_SHORT_TWICE,
    FOUR_VERY_SHORT,
    SHORT_AND_LONG,
    LONG_AND_TWO_VERY_SHORT_AND_LONG,
    LED_MODES_NUMBER
};

void LEDModeExecution(enum mode SelectedMode);

void LEDModeContinuation(void);
