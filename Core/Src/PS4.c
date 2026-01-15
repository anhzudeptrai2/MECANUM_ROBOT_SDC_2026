#include "PS4.h"
#include "main.h"

#include <string.h>

UART_HandleTypeDef *huart_ps4;

PS4_DATA PS4_Dat;
volatile BS Button_State = BUTTON_NONE;

uint8_t rx_buffer[sizeof(PS4_DATA)];

void PS4_Init(UART_HandleTypeDef *uart_in)
{
    huart_ps4 = uart_in;
}

static void Extract_Button(uint16_t button_val, volatile BS *extracted_button)
{
    *extracted_button = (BS)button_val;
}

void PS4_UART_Req(void)
{
#if defined(HAL_UART_MODULE_ENABLED)
    uint8_t Req_Char = 'p';
    HAL_UART_Transmit(huart_ps4, &Req_Char, 1, HAL_MAX_DELAY);
    HAL_UART_Receive_DMA(huart_ps4, rx_buffer, sizeof(PS4_Dat));
#else
    (void)huart_ps4;
#endif
}

void PS4_UART_Rx_IDLE_Handle(void)
{
    if (rx_buffer[8] == 0xff)
    {
        memcpy(&PS4_Dat, rx_buffer, sizeof(PS4_Dat));
        Extract_Button(PS4_Dat.button, &Button_State);
    }
    else
    {
        memset(rx_buffer, 0, sizeof(rx_buffer));
    }
}
