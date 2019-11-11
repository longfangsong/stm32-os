#ifndef STM32_OS_UART_H
#define STM32_OS_UART_H

void uart_send(unsigned char *data, size_t length);

void uart_receive(unsigned char *data, size_t length);

#endif //STM32_OS_UART_H
