#include "stm32f1xx_hal.h"
#include "../panic/panic.h"
#include "init.h"
#include "../heap/heap.h"
#include "../../device/lcd/lcd_basic.h"
#include "../thread/scheduler/scheduler.h"
#include "../../device/led/led.h"

void interrupt_init() {
    HAL_NVIC_SetPriority(PendSV_IRQn, 0xf, 0xf);
}

EXPORT_BOARD_INIT(interrupt_init);

static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        panic_with_message("RCC");
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        panic_with_message("RCC");
    }
}

EXPORT_BOARD_INIT(SystemClock_Config);

//static void MX_USART1_UART_Init(void) {
//    huart1.Instance = USART1;
//    huart1.Init.BaudRate = 115200;
//    huart1.Init.WordLength = UART_WORDLENGTH_8B;
//    huart1.Init.StopBits = UART_STOPBITS_1;
//    huart1.Init.Parity = UART_PARITY_NONE;
//    huart1.Init.Mode = UART_MODE_TX_RX;
//    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//    if (HAL_UART_Init(&huart1) != HAL_OK) {
//        panic_with_message("uart");
//    }
//}
//
//EXPORT_BOARD_INIT(MX_USART1_UART_Init);

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
}

EXPORT_BOARD_INIT(MX_GPIO_Init);

void init_all() {
    extern init_function __init_functions_start;
    extern init_function __init_functions_end;
    HAL_Init();
    for (init_function *init = &__init_functions_start; init != &__init_functions_end; ++init) {
        (*init)();
    }
}