#ifndef DRIVER_PID_AML_H
#define DRIVER_PID_AML_H

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration to avoid depending on stm32xx_hal_uart.h in headers */
typedef struct __UART_HandleTypeDef UART_HandleTypeDef;

#define MAX_UARTS 4

typedef struct
{
    uint8_t ID;
    volatile int16_t Set_Point;
    UART_HandleTypeDef *Driver_UART;
} Motor_Driver;

void Driver_PID_AML_Init_UART(UART_HandleTypeDef *uart_par, Motor_Driver *motor);
void Assign_PID_AML_Id(Motor_Driver *motor, uint8_t id);
void Driver_Home_Request(Motor_Driver motor);
void Driver_Set_Zero_Position(Motor_Driver motor);
void Driver_Send_Setpoints_U1(Motor_Driver *motors, uint8_t num_motors);
// void Driver_Send_Setpoints_U2(Motor_Driver *motors, uint8_t num_motors);
#endif // DRIVER_PID_AML_H
