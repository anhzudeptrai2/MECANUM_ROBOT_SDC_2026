#ifndef STM32H7XX_HAL_UART_H
#define STM32H7XX_HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Important include-order note:
 * Do NOT include stm32h7xx_hal_def.h first here.
 * stm32h7xx_hal_def.h includes stm32h7xx.h which (with USE_HAL_DRIVER) includes
 * stm32h7xx_hal.h; if stm32h7xx_hal.h hasn't had its include-guard set yet,
 * that creates an include cycle where HAL types aren't defined in time.
 */
#include "stm32h7xx_hal.h"

/* Minimal UART HAL subset to satisfy this project.
 * Note: This is not a full STM32H7 HAL UART implementation.
 */

typedef struct
{
  uint32_t AdvFeatureInit;
} UART_AdvFeatureInitTypeDef;

typedef struct
{
  uint32_t BaudRate;
  uint32_t WordLength;
  uint32_t StopBits;
  uint32_t Parity;
  uint32_t Mode;
  uint32_t HwFlowCtl;
  uint32_t OverSampling;
  uint32_t OneBitSampling;
  uint32_t ClockPrescaler;
} UART_InitTypeDef;

typedef enum
{
  HAL_UART_STATE_RESET             = 0x00U,
  HAL_UART_STATE_READY             = 0x20U,
  HAL_UART_STATE_BUSY              = 0x24U,
  HAL_UART_STATE_BUSY_TX           = 0x21U,
  HAL_UART_STATE_BUSY_RX           = 0x22U,
  HAL_UART_STATE_BUSY_TX_RX        = 0x23U,
  HAL_UART_STATE_TIMEOUT           = 0xA0U,
  HAL_UART_STATE_ERROR             = 0xE0U
} HAL_UART_StateTypeDef;

typedef struct __UART_HandleTypeDef
{
  USART_TypeDef                 *Instance;
  UART_InitTypeDef               Init;
  UART_AdvFeatureInitTypeDef     AdvancedInit;

  DMA_HandleTypeDef             *hdmatx;
  DMA_HandleTypeDef             *hdmarx;

  HAL_LockTypeDef                Lock;
  __IO uint32_t                  State;
} UART_HandleTypeDef;

/* Common configuration values used by CubeMX-generated code */
#define UART_WORDLENGTH_8B              0x00000000U
#define UART_STOPBITS_1                 0x00000000U
#define UART_PARITY_NONE                0x00000000U
#define UART_MODE_TX_RX                 0x00000003U
#define UART_HWCONTROL_NONE             0x00000000U
#define UART_OVERSAMPLING_16            0x00000000U
#define UART_ONE_BIT_SAMPLE_DISABLE     0x00000000U
#define UART_PRESCALER_DIV1             0x00000000U
#define UART_ADVFEATURE_NO_INIT         0x00000000U

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

/* CubeMX-generated code expects UARTEx APIs/macros when including this header. */
#include "stm32h7xx_hal_uart_ex.h"

#ifdef __cplusplus
}
#endif

#endif /* STM32H7XX_HAL_UART_H */
