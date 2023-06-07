#ifndef GBA_SRAM_H
#define GBA_SRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read data from SRAM.
 *
 * @param addr address in SRAM (unit: byte).
 * @param data buffer to hold data.
 * @param size data size (unit: byte).
 *
 * @return \c 0 for success, \c non-zero for error. 
 */
int sram_read(u32 addr, u8 *data, size_t size);

/**
 * @brief Write data to SRAM.
 *
 * @param addr address in SRAM (unit: byte).
 * @param data buffer to hold data.
 * @param size data size (unit: byte).
 *
 * @return \c 0 for success, \c non-zero for error.
 */
int sram_write(u32 addr, u8 *data, size_t size);

#ifdef __cplusplus
};
#endif

#endif /* GBA_SRAM_H */
