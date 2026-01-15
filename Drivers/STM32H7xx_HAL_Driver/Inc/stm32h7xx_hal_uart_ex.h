#ifndef STM32H7XX_HAL_UART_EX_H
#define STM32H7XX_HAL_UART_EX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal_uart.h"

/* Minimal UARTEx subset for this project.
 * Note: This is not a full STM32H7 HAL UARTEx implementation.
 */

#define UART_TXFIFO_THRESHOLD_1_8 0x00000000U
#define UART_RXFIFO_THRESHOLD_1_8 0x00000000U

HAL_StatusTypeDef HAL_UARTEx_SetTxFifoThreshold(UART_HandleTypeDef *huart, uint32_t Threshold);
HAL_StatusTypeDef HAL_UARTEx_SetRxFifoThreshold(UART_HandleTypeDef *huart, uint32_t Threshold);
HAL_StatusTypeDef HAL_UARTEx_DisableFifoMode(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* STM32H7XX_HAL_UART_EX_H */
