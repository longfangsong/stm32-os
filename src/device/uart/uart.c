#include "stm32f1xx_hal.h"
#include "uart.h"
#include "../../kernel/panic/panic.h"
#include "../../kernel/init/init.h"

UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        panic_with_message("uart");
    }
}

EXPORT_DEVICE_INIT(MX_USART1_UART_Init);

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

inline void uart_send(unsigned char *data, size_t length) {
    HAL_UART_Transmit(&huart1, data, length, 1000);
}

inline void uart_receive(unsigned char *data, size_t length) {
    HAL_UART_Receive(&huart1, data, length, 1000);
}
