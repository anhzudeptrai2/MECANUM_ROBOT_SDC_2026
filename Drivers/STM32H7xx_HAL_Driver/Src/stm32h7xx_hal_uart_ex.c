#include "stm32h7xx_hal_uart_ex.h"

HAL_StatusTypeDef HAL_UARTEx_SetTxFifoThreshold(UART_HandleTypeDef *huart, uint32_t Threshold)
{
  UNUSED(huart);
  UNUSED(Threshold);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_UARTEx_SetRxFifoThreshold(UART_HandleTypeDef *huart, uint32_t Threshold)
{
  UNUSED(huart);
  UNUSED(Threshold);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_UARTEx_DisableFifoMode(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
  return HAL_OK;
}
