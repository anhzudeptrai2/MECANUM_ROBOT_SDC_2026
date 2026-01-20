#include "WT901C.h"
#include "CRC16.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
// Can cau hinh UART mode Rx DMA normal!

#include <string.h>

#ifdef TTL_MODE
// Buffer phuc vu khoi phuc frame bi cat
static uint8_t overflowBuffer[FRAME_LEN];
static uint8_t overflowLength = 0;
#endif

// ================== Buffer Nh?n D? Li?u ==================
// Buffer DMA nhan du lieu tu WT901C
uint8_t rx_buffer_imu[RX_BUFFER_SIZE];
// Buffer tam de xu ly truong hop frame du lieu bi cat

// ================== BUFFER L?NH ==================
// Buffer gui yeu cau cap nhat goc
#ifdef RS485_MODE
static const uint8_t ANGLE_RQST_BUFFER[] = {0x55, 0x00, 0x00, 0x3D, 0x00, 0x03, 0x94, 0x07};
#endif

// Buffer gui lenh reset goc (doi voi TTL mode)
static const uint8_t tx_angle_rst_buffer[ANGLE_RESET_BUFFER_SIZE] = {
    0xFF, 0xAA, 0x52, 0x00, 0x00,
    0xFF, 0xAA, 0x52, 0x01, 0x00,
    0xFF, 0xAA, 0x52, 0x02, 0x00};

// ================== BIEN DOI GOC YAW ==================
static float Yaw_Continuous_Angle = 0.0f;
static int16_t Yaw_Roll_Count = 0;
static float Prev_Yaw_Angle = 0.0f;
static float offset_angle = 0.0f;

#ifdef TTL_MODE
/**
 * @brief Tinh checksum cho frame du lieu
 * @param frame Con tro toi frame du lieu
 * @param length Do dai cua frame (bao gom checksum cuoi)
 * @return Gia tri checksum tinh duoc
 */
static uint8_t computeChecksum(const uint8_t *frame, uint8_t length)
{
    uint8_t cs = 0;
    for (uint8_t i = 0; i < length - 1; i++)
        cs += frame[i];
    return cs;
}
#endif

/**
 * @brief Cap nhat gia tri goc yaw lien tuc (khong bi gioi han trong [0,360])
 * @param current_angle Goc yaw hien tai
 */
static void Infinite_Yaw(float current_angle)
{
    float delta = current_angle - Prev_Yaw_Angle;
    if (delta > 180.0f)
        Yaw_Roll_Count--;
    else if (delta < -180.0f)
        Yaw_Roll_Count++;

    Yaw_Continuous_Angle = current_angle + (Yaw_Roll_Count * 360.0f);
    Prev_Yaw_Angle = current_angle;
}

// ================== KHOI TAO VA HIEN THUC ==================

void WT901C_Init(WT901C *imu, UART_HandleTypeDef *wt901_uart)
{
    imu->WT901C_UART = wt901_uart;
    imu->Yaw = imu->Roll = imu->Pitch = 0.0f;

    WT901C_Calibrate_Accel(imu);
    HAL_Delay(100);
    WT901C_Reset_Angles(imu);
    HAL_Delay(100);
    WT901C_Begin_Recieve(imu);
}

void WT901C_Reset_Angles(WT901C *imu)
{
    WT901C_Stop_Recieve(imu);
#ifdef TTL_MODE
    // Gui lenh reset goc theo tung phan (moi phan 5 byte)
    for (int i = 0; i < ANGLE_RESET_BUFFER_SIZE; i += 5)
    {
        HAL_UART_Transmit(imu->WT901C_UART, (uint8_t *)(tx_angle_rst_buffer + i), 5, 100);
        HAL_Delay(2);
    }
#elif defined RS485_MODE
    HAL_UART_Transmit_DMA(imu->WT901C_UART, (uint8_t *)tx_angle_rst_buffer, ANGLE_RESET_BUFFER_SIZE);
#endif

    // Cap nhat lai offset va reset cac bien lien quan den yaw
    offset_angle = Yaw_Continuous_Angle;
    Yaw_Roll_Count = 0;
    Prev_Yaw_Angle = 0.0f;
    Yaw_Continuous_Angle = 0.0f;
    imu->Yaw = imu->Roll = imu->Pitch = 0.0f;

    WT901C_Begin_Recieve(imu);
}

void WT901C_Start_Receive(WT901C *imu)
{
    HAL_UART_Receive_DMA(imu->WT901C_UART, rx_buffer_imu, RX_BUFFER_SIZE);
}

void WT901C_Begin_Recieve(WT901C *imu)
{
#ifdef TTL_MODE
    WT901C_Start_Receive(imu);
#elif defined RS485_MODE
    WT901C_Angle_Request(imu);
#endif
}

void WT901C_Stop_Recieve(WT901C *imu)
{
    if ((imu == 0) || (imu->WT901C_UART == 0))
    {
        return;
    }

    if (imu->WT901C_UART->hdmarx != 0)
    {
        (void)HAL_DMA_Abort(imu->WT901C_UART->hdmarx);
    }
}

#ifdef RS485_MODE
void WT901C_Angle_Request(WT901C *imu)
{
    HAL_UART_Transmit_DMA(imu->WT901C_UART, (uint8_t *)ANGLE_RQST_BUFFER, sizeof(ANGLE_RQST_BUFFER));
    HAL_UART_Receive_DMA(imu->WT901C_UART, rx_buffer_imu, RX_BUFFER_SIZE);
}
#endif

/**
 * @brief Xu ly buffer du lieu nhan duoc tu UART, tach cac frame hop le va cap nhat cac goc.
 * @param imu Con tro toi cau truc WT901C
 * @param buffer Buffer du lieu nhan duoc
 * @param length So byte nhan duoc
 */
void WT901C_Process_Buffer(WT901C *imu, uint8_t *buffer, uint16_t length)
{
    uint8_t tempBuffer[RX_BUFFER_SIZE + FRAME_LEN];
    uint16_t totalLength = 0;

    // Neu co du lieu du tu lan nhan truoc, copy sang tempBuffer
    if (overflowLength > 0)
    {
        memcpy(tempBuffer, overflowBuffer, overflowLength);
        totalLength = overflowLength;
        overflowLength = 0;
    }

    // Copy du lieu moi nhan vao tempBuffer
    memcpy(&tempBuffer[totalLength], buffer, length);
    totalLength += length;

    uint16_t i = 0;
    while (i <= totalLength - FRAME_LEN)
    {
        if (tempBuffer[i] == 0x55 && tempBuffer[i + 1] == 0x53)
        {
#ifdef TTL_MODE
            if (computeChecksum(&tempBuffer[i], FRAME_LEN) == tempBuffer[i + FRAME_LEN - 1])
#else
            uint16_t frameCrc = (tempBuffer[i + FRAME_LEN - 1] << 8) | tempBuffer[i + FRAME_LEN - 2];
            if (CRC_16(&tempBuffer[i], FRAME_LEN - 2) == frameCrc)
#endif
            {
                // Giai ma goc Roll, Pitch va Yaw
                imu->Roll = ((int16_t)((tempBuffer[i + 3] << 8) | tempBuffer[i + 4])) * (180.0f / 32768.0f);
                imu->Pitch = ((int16_t)((tempBuffer[i + 5] << 8) | tempBuffer[i + 6])) * (180.0f / 32768.0f);
                float currentYaw = ((int16_t)((tempBuffer[i + 7] << 8) | tempBuffer[i + 8])) * (180.0f / 32768.0f);
                Infinite_Yaw(currentYaw);
                imu->Yaw = Yaw_Continuous_Angle - offset_angle;
                i += FRAME_LEN;
                continue;
            }
        }
        i++;
    }
    // Luu lai du lieu du (neu frame chua hoan chinh) de xu ly o lan nhan sau
    if (i < totalLength)
    {
        overflowLength = totalLength - i;
        memcpy(overflowBuffer, &tempBuffer[i], overflowLength);
    }
}

/**
 * @brief Callback xu ly khi UART nhan duoc trang thai IDLE.
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_UART_Rx_IDLE_Hanlde(WT901C *imu)
{
    WT901C_Process_Buffer(imu, rx_buffer_imu, RX_BUFFER_SIZE);
#ifdef TTL_MODE
    WT901C_Start_Receive(imu);
#endif
}

/**
 * @brief Hieu chuan gia toc cua cam bien WT901C.
 * @param imu Con tro toi cau truc WT901C
 */
void WT901C_Calibrate_Accel(WT901C *imu)
{
    const uint8_t cmd1[] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
    const uint8_t cmd2[] = {0xFF, 0xAA, 0x01, 0x01, 0x00};
    const uint8_t cmd3[] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
    const uint8_t cmd4[] = {0xFF, 0xAA, 0x00, 0x00, 0x00};

    WT901C_Stop_Recieve(imu);

    HAL_UART_Transmit(imu->WT901C_UART, (uint8_t *)cmd1, sizeof(cmd1), 100);
    HAL_Delay(100);
    HAL_UART_Transmit(imu->WT901C_UART, (uint8_t *)cmd2, sizeof(cmd2), 100);
    HAL_Delay(10000);
    HAL_UART_Transmit(imu->WT901C_UART, (uint8_t *)cmd3, sizeof(cmd3), 100);
    HAL_Delay(100);
    HAL_UART_Transmit(imu->WT901C_UART, (uint8_t *)cmd4, sizeof(cmd4), 100);
    HAL_Delay(100);
}
