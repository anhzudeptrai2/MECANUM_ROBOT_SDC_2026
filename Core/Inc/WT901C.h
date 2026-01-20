#ifndef _WT901C_H_
#define _WT901C_H_

#include "stdint.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"

#define WT901C_REQ_MAX_TIME 100 // ms

#define TTL_MODE

static inline uint8_t Low_Byte(uint16_t w) { return (uint8_t)(w & 0x00FF); }

static inline uint8_t High_Byte(uint16_t w) { return (uint8_t)((w >> 8) & 0x00FF); }
// Cau truc du lieu cho cam bien WT901C
typedef struct {
  float Roll;
  float Pitch;
  float Yaw;
  UART_HandleTypeDef *WT901C_UART;
} WT901C;
// Danh cho giao thuc MODBUS RTU (RS485)
#define WT901C_ADDR 0x01
#define WT901C_ADDC WT901C_ADDR
#define READ_FUNC 0x03
#define WRITE_FUNC 0x06

#if defined(RS485_MODE)
#define RX_BUFFER_SIZE 11
#define ANGLE_RESET_BUFFER_SIZE 24
#define ANGLE_RESET_COMMAND                                                    \
  {WT901C_ADDR, WRITE_FUNC, 0x00, 0x69, 0xB5, 0x88, 0x2F, 0x20,                \
   WT901C_ADDR, WRITE_FUNC, 0x00, 0x01, 0x00, 0x04, 0xD9, 0xC9,                \
   WT901C_ADDR, WRITE_FUNC, 0x00, 0x00, 0x00, 0x00, 0x89, 0xCA}
#elif defined(TTL_MODE)
// Cau hinh frame cho TTL UART
#define RX_BUFFER_SIZE 25
#define ANGLE_RESET_BUFFER_SIZE 15
#define FRAME_LEN 11
#define ANGLE_RESET_COMMAND                                                    \
  {0xFF, 0xAA, 0x69, 0x88, 0xB5, 0xFF, 0xAA, 0x01,                             \
   0x04, 0x00, 0xFF, 0xAA, 0x00, 0x00, 0x00}
#endif

/*================= Public Function Prototypes =================*/

/**
 * @brief Khoi tao cam bien WT901C voi UART handler va bat dau nhan du lieu
 * @param imu Con tro toi cau truc WT901C
 * @param wt901_uart Con tro toi UART handler su dung cho WT901C
 */
void WT901C_Init(WT901C *imu, UART_HandleTypeDef *wt901_uart);

/**
 * @brief Reset cac goc cua cam bien WT901C
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Reset_Angles(WT901C *imu);

#ifdef RS485_MODE
/**
 * @brief Gui yeu cau cap nhat goc (su dung trong Timer interrupt)
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Angle_Request(WT901C *imu);
#endif

/**
 * @brief Bat dau nhan du lieu tu cam bien
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Begin_Recieve(WT901C *imu);

/**
 * @brief Xu ly callback khi UART nhan duoc trang thai IDLE
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_UART_Rx_IDLE_Hanlde(WT901C *imu);

/**
 * @brief Hieu chuan gia toc cua cam bien WT901C
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Calibrate_Accel(WT901C *imu);

/**
 * @brief Dung nhan du lieu tu cam bien
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Stop_Recieve(WT901C *imu);
void WTC901C_Start_Receive(WT901C *imu);
#endif // WT901C_H
