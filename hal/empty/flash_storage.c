/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-29     supperthomas first version
 *
 */

#include "flash_storage.h"
#include "ble_qiot_config.h"
//FLASH not init first

void fstorage_init(void)
{
    
}

int fstorage_read(uint32_t addr, uint32_t read_len, void *p_data)
{
     return read_len;
}

int fstorage_erase(uint32_t addr)
{
    return 0;
}

int fstorage_write(uint32_t addr, uint32_t write_len, void const *p_data)
{
    return write_len;
}


