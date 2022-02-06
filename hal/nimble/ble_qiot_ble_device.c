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

#include "flash_storage.h"
#ifdef PKG_USING_NIMBLE
#include "host/ble_gap.h"
#endif

// divece info which defined in explorer platform
#define PRODUCT_ID  "05SAS7U5N4"
#define DEVICE_NAME "ble"
#define SECRET_KEY  "/Fyy1W2oD6Bup4lojNsKFw=="

static uint8_t m_adv_handle = NULL; /**< Advertising handle used to identify an advertising set. */
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
    uint8_t addr_le[6];
    int rc = ble_hs_id_copy_addr(BLE_ADDR_PUBLIC, addr_le, NULL);
    mac[0] = addr_le[5];
    mac[1] = addr_le[4];
    mac[2] = addr_le[3];
    mac[3] = addr_le[2];
    mac[4] = addr_le[1];
    mac[5] = addr_le[0];
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



ble_timer_t ble_timer_create(uint8_t type, ble_timer_cb timeout_handle)
{
    MODLOG_DFLT(INFO, "===%s==%d=====\r\n", __func__, __LINE__);
    return 0;
}


ble_qiot_ret_status_t ble_timer_start(ble_timer_t timer_id, uint32_t period)
{
    MODLOG_DFLT(INFO, "===%s==%d=====\r\n", __func__, __LINE__);
    return 0;
}

ble_qiot_ret_status_t ble_timer_stop(ble_timer_t timer_id)
{
    MODLOG_DFLT(INFO, "===%s==%d=====\r\n", __func__, __LINE__);
    return 0;
}

ble_qiot_ret_status_t ble_timer_delete(ble_timer_t timer_id)
{
    MODLOG_DFLT(INFO, "===%s==%d=====\r\n", __func__, __LINE__);
    // do nothing
    return BLE_QIOT_RS_OK;
}



//================================================

ble_qiot_ret_status_t ble_advertising_start(adv_info_s *adv)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;


    /*
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info)
     *     o Advertising tx power
     *     o Device name
     */
    memset(&fields, 0, sizeof(fields));

    ble_uuid16_t   uuids;
    uuids.value                       = IOT_BLE_UUID_SERVICE;
    uuids.u.type = BLE_UUID_TYPE_16;
    fields.uuids16 = &uuids;
    fields.num_uuids16 = 1;
    fields.uuids16_is_complete = 1;

    rc = ble_gap_adv_rsp_set_fields(&fields);
    if (rc != 0)
    {
        MODLOG_DFLT(INFO, "error setting advertisement data; rc=%d\n", rc);
        //    return;
    }
    memset(&fields, 0, sizeof(fields));

    /*
     * Advertise two flags:
     *      o Discoverability in forthcoming advertisement (general)
     *      o BLE-only (BR/EDR unsupported)
     */
    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;

    fields.name = (uint8_t *)DEVICE_NAME;
    fields.name_len = strlen(DEVICE_NAME);
    fields.name_is_complete = 1;

    for (int i = 0; i < adv->manufacturer_info.adv_data_len; i++)
    {
        adv->manufacturer_info.adv_data[adv->manufacturer_info.adv_data_len - i + 1] =  adv->manufacturer_info.adv_data[adv->manufacturer_info.adv_data_len - i - 1];
    }
    adv->manufacturer_info.adv_data[0] = adv->manufacturer_info.company_identifier & 0xff;
    adv->manufacturer_info.adv_data[1] = (uint8_t)(adv->manufacturer_info.company_identifier >> 8) & 0xff;
    fields.mfg_data = adv->manufacturer_info.adv_data;
    fields.mfg_data_len = adv->manufacturer_info.adv_data_len + 2;


    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0)
    {
        MODLOG_DFLT(INFO, "error setting advertisement data; rc=%d\n", rc);
        //    return;
    }

    /* Begin advertising */
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    extern uint8_t blehr_addr_type;
    extern int blehr_gap_event(struct ble_gap_event * event, void *arg);
    rc = ble_gap_adv_start(blehr_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, blehr_gap_event, NULL);
    if (rc != 0)
    {
        MODLOG_DFLT(INFO, "error enabling advertisement; rc=%d\n", rc);
        //return;
    }

    return 0;
}

ble_qiot_ret_status_t ble_advertising_stop(void)
{
    ble_gap_adv_stop();
    MODLOG_DFLT(INFO, "===%s==%d=====\r\n", __func__, __LINE__);
    return BLE_QIOT_RS_OK;
}
extern uint16_t notify_conn_handle;
ble_qiot_ret_status_t ble_send_notify(uint8_t *buf, uint8_t len)
{

    MODLOG_DFLT(INFO, "===%s==%d==len:%x==notify_conn_handle:%x=\r\n", __func__, __LINE__, len, notify_conn_handle);
    struct os_mbuf *om;
    om = ble_hs_mbuf_from_flat(buf, len);

    ble_gattc_notify_custom(notify_conn_handle, 7, om);

    return BLE_QIOT_RS_OK;
}

// should return ATT_MTU - 3
uint16_t ble_get_user_data_mtu_size(void)
{
    uint16_t mtu = ble_att_mtu(notify_conn_handle);
    MODLOG_DFLT(INFO, "===%s==%d=mtu:%x====\r\n", __func__, __LINE__,mtu);
    return mtu;
}

void property_power_switch(const char *data, uint16_t len)
{
    MODLOG_DFLT(INFO, "===%s==%d==len:%x=data:%x==\r\n", __func__, __LINE__, len, data[0]);

#define DK_BOARD_LED_2  18

#ifdef __RTTHREAD__
    rt_pin_write(DK_BOARD_LED_2, data[0]);
#endif
    return;
}

void action_led_blink(int ms)
{
    MODLOG_DFLT(INFO, "===%s==%d====\n", __func__, __LINE__);
}

void report_reply_blink(void)
{
    MODLOG_DFLT(INFO, "===%s==%d====\n", __func__, __LINE__);
}

#ifdef __cplusplus
}
#endif
