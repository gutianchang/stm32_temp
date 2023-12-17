#ifndef STM32_CMAKE_EXAMPLES_FREERTOS_MAIN_H_
#define STM32_CMAKE_EXAMPLES_FREERTOS_MAIN_H_

namespace blinky {

void init();

#if defined STM32F7
static void blueLEDBlinkTask(void* args);
static void redLEDBlinkTask(void* args);
static void greenLEDBlinkTask(void* args);
#else
static void blinkTask(void* args);
#endif

}

#endif /* STM32_CMAKE_EXAMPLES_FREERTOS_MAIN_H_ */
