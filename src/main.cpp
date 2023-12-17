#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#if defined STM32F1
    #include <stm32f1xx_hal.h>

    // STM32VL-Discovery green led - PC9
    #define LED_PORT                GPIOC
    #define LED_PIN                 GPIO_PIN_9
    // STM32VL-Discovery blue led - PC8
    //#define LED_PIN                 GPIO_PIN_8
    #define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE
#elif defined STM32F7
    #include <stm32f7xx_hal.h>

    // PIXHAWK mini V4 blue led - PC7
    #define BLUE_LED_PORT                GPIOC
    #define BLUE_LED_PIN                 GPIO_PIN_7
    #define BLUE_LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE

    #define RED_LED_PORT                GPIOB
    #define RED_LED_PIN                 GPIO_PIN_1
    #define RED_LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE

    #define GREEN_LED_PORT                GPIOC
    #define GREEN_LED_PIN                 GPIO_PIN_6
    #define GREEN_LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE
#endif



#if defined STM32F7
static void blinky::blueLEDBlinkTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(BLUE_LED_PORT, BLUE_LED_PIN);
    }
}

static void blinky::redLEDBlinkTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
    }
}

static void blinky::greenLEDBlinkTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(GREEN_LED_PORT, GREEN_LED_PIN);
    }
}
#else
    static void blinky::blinkTask(void *arg)
    {
        for(;;)
        {
            vTaskDelay(500);
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        }
    }
#endif

void blinky::init()
{
#if defined STM32F7
    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Config.Pin = BLUE_LED_PIN | GREEN_LED_PIN;
    BLUE_LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(BLUE_LED_PORT, &GPIO_Config);

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Config.Pin = RED_LED_PIN;
    RED_LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(RED_LED_PORT, &GPIO_Config);

#else
    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_Config.Pin = LED_PIN;

    LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &GPIO_Config);
#endif
}

int main(void)
{
    SystemInit();
    blinky::init();
    
    
#if defined STM32F7
    xTaskCreate(blinky::blueLEDBlinkTask, "blueLEDBlinkTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blinky::redLEDBlinkTask, "redLEDBlinkTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blinky::greenLEDBlinkTask, "greenLEDBlinkTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#else 
    xTaskCreate(blinky::blinkTask, "blinky", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
#endif
 
    vTaskStartScheduler();
    for (;;);
    
    return 0;
}

extern "C" void vApplicationTickHook(void)
{
}

extern "C" void vApplicationIdleHook(void)
{
}

extern "C" void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    taskDISABLE_INTERRUPTS();
    for(;;);
}
