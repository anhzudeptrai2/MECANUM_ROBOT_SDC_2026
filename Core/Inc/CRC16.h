#ifndef __CRC16_H__
#define __CRC16_H__

#include <stdint.h>
#include <stddef.h>

uint16_t CRC_16(uint8_t *buf, size_t len);

#endif // CRC16_H