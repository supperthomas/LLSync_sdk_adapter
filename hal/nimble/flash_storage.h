/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-29     supperthomas first version
 *
 */

#include <stdint.h>

/**
 * @brief initialize of flash storage
 *
 */
void fstorage_init(void);

/**
 * @brief read data from flash
 *
 * @param addr read address from flash
 * @param read_len length of data to read
 * @param p_data point to read buf
 * @return read length is success, other is error
 */
int fstorage_read(uint32_t addr, uint32_t read_len, void *p_data);

/**
 * @brief write data to flash, must erase before write
 *
 * @param addr write address in flash
 * @param write_len length of data to write
 * @param p_data point to write buf
 * @return write length is success, other is error
 */
int fstorage_write(uint32_t addr, uint32_t write_len, void const *p_data);

/**
 * @brief erase one page
 *
 * @param addr address must align to page. For example, one page is 4096 bytes,
 * you want to erase address 0x1020, you should set address 0x1000
 * @return 0 is succcess, other is error
 */
int fstorage_erase(uint32_t addr);
