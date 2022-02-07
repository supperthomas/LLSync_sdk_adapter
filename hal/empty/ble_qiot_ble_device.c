/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-29     supperthomas first version
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ble_qiot_export.h"
#include "ble_qiot_service.h"
#include "ble_qiot_import.h"

// add std head file here
#include <stdint.h>

// add ble qiot head file here
#include "ble_qiot_import.h"

// add sdk head file here
#include "board.h"

#include "flash_storage.h"

// divece info which defined in explorer platform
#define PRODUCT_ID  "05SAS7U5N4"
#define DEVICE_NAME "ble"
#define SECRET_KEY  "/Fyy1W2oD6Bup4lojNsKFw=="

static uint8_t m_adv_handle =NULL; /**< Advertising handle used to identify an advertising set. */
static uint8_t m_enc_advdata[31]; /**< Buffer for storing an encoded advertising set. */
static uint8_t m_enc_scan_response_data[31]; /**< Buffer for storing an encoded scan data. */


int ble_get_product_id(char *product_id)
{
    memcpy(product_id, PRODUCT_ID, strlen(PRODUCT_ID));

    return 0;
}

int ble_get_device_name(char *device_name)
{
    memcpy(device_name, DEVICE_NAME, strlen(DEVICE_NAME));

    return strlen(DEVICE_NAME);
}

int ble_get_psk(char *psk)
{
    memcpy(psk, SECRET_KEY, strlen(SECRET_KEY));

    return 0;
}

int ble_get_mac(char *mac)
{
    return 0;
}

int ble_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len)
{
    return fstorage_write(flash_addr, write_len, write_buf);
}

int ble_read_flash(uint32_t flash_addr, char *read_buf, uint16_t read_len)
{
    return fstorage_read(flash_addr, read_len, read_buf);
}

ble_qiot_ret_status_t ble_advertising_start(adv_info_s *adv)
{
    return 0;
}

ble_qiot_ret_status_t ble_advertising_stop(void)
{
    return BLE_QIOT_RS_OK;
}


ble_timer_t ble_timer_create(uint8_t type, ble_timer_cb timeout_handle)
{
    return 0;
}


ble_qiot_ret_status_t ble_timer_start(ble_timer_t timer_id, uint32_t period)
{
    return 0;
}

ble_qiot_ret_status_t ble_timer_stop(ble_timer_t timer_id)
{
    return 0;
}

ble_qiot_ret_status_t ble_timer_delete(ble_timer_t timer_id)
{
    // do nothing
    return BLE_QIOT_RS_OK;
}

ble_qiot_ret_status_t ble_send_notify(uint8_t *buf, uint8_t len)
{

    return BLE_QIOT_RS_OK;
}

// should return ATT_MTU - 3
uint16_t ble_get_user_data_mtu_size(void)
{
   
    return 0;
}

void property_power_switch(const char *data, uint16_t len)
{
    
    return;
}

void action_led_blink(int ms)
{
  
}

void report_reply_blink(void)
{
   
}

#ifdef __cplusplus
}
#endif
