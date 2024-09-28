#ifndef CRC_H_
#define CRC_H_

uint8_t u8_rev(uint8_t x);
uint32_t u32_rev(uint32_t x);
uint32_t crc32_update(uint32_t x0, uint8_t *b, uint8_t n);

#endif
