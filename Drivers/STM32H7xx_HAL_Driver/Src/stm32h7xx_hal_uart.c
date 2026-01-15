#include "stm32h7xx_hal_uart.h"

/* Weak MSP hooks (CubeMX provides strong definitions in stm32h7xx_hal_msp.c) */
__weak void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
}

__weak void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
}

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  HAL_UART_MspInit(huart);
  return HAL_OK;
}

void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
}

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  UNUSED(huart);
  UNUSED(pData);
  UNUSED(Size);
  UNUSED(Timeout);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  UNUSED(huart);
  UNUSED(pData);
  UNUSED(Size);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  UNUSED(huart);
  UNUSED(pData);
  UNUSED(Size);
  return HAL_OK;
}
