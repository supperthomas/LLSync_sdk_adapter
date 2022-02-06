/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-29     supperthomas first version
 *
 */

#include "mcu_service.h"
#ifdef __RTTHREAD__
#include "rtthread.h"
#endif
#include <stdint.h>
#include <string.h>


#include "ble_qiot_export.h"
#include "ble_qiot_service.h"
#include "ble_qiot_import.h"
#ifdef PKG_USING_NIMBLE
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "nimble/ble.h"
#endif


//IOT_BLE_UUID_BASE + IOT_BLE_UUID_SERVICE
#define SERVICE_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE0, 0xFF, 0x00, 0x00


//IOT_BLE_UUID_BASE + IOT_BLE_UUID_DEVICE_INFO
#define SDEVICE_INFO_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE1, 0xFF, 0x00, 0x00


//IOT_BLE_UUID_BASE + IOT_BLE_UUID_DATA
#define SDEVICE_DATA_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE2, 0xFF, 0x00, 0x00

//IOT_BLE_UUID_BASE + IOT_BLE_UUID_EVENT
#define SDEVICE_DATA_EVENT                                                                           \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE3, 0xFF, 0x00, 0x00


static uint16_t device_info_handle;
static uint16_t device_data_handle;
static uint16_t device_event_handle;


//============================service====================
static int
gatt_svr_chr_access_q_device_info(uint16_t conn_handle, uint16_t attr_handle,
                                  struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    MODLOG_DFLT(INFO, "===%s==%d====\n", __func__, __LINE__);
    uint16_t uuid;
    uuid = ble_uuid_u16(ctxt->chr->uuid);
    uint8_t data[32];
    os_mbuf_copydata(ctxt->om, 0, ctxt->om->om_len, data);

    ble_device_info_write_cb(data, ctxt->om->om_len);
    return 0;
}

static int
gatt_svr_chr_access_device_data(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */
    MODLOG_DFLT(INFO, "===%s==%d====\n", __func__, __LINE__);
    uint16_t uuid;
    uuid = ble_uuid_u16(ctxt->chr->uuid);
    uint8_t data[32];
    os_mbuf_copydata(ctxt->om, 0, ctxt->om->om_len, data);
    ble_lldata_write_cb(data, ctxt->om->om_len);

    return 0;
}

static int
gatt_svr_chr_access_event_cb(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    MODLOG_DFLT(INFO, "===%s==%d====\n", __func__, __LINE__);
    return 0;
}

static const struct ble_gatt_svc_def gatt_svr_svcs[] =
{
    {
        /* Service: Heart-rate */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID128_DECLARE(SERVICE_BASE),
        .characteristics = (struct ble_gatt_chr_def[])
        {
            {
                /* Characteristic: Heart-rate measurement */
                .uuid = BLE_UUID128_DECLARE(SDEVICE_INFO_BASE),
                .access_cb = gatt_svr_chr_access_q_device_info,
                .val_handle = &device_info_handle,
                .flags = BLE_GATT_CHR_F_WRITE,
            },
            {
                /* Characteristic: Body sensor location */
                .uuid = BLE_UUID128_DECLARE(SDEVICE_DATA_BASE),
                .access_cb = gatt_svr_chr_access_device_data,
                .flags = BLE_GATT_CHR_F_WRITE,
            },
            {
                /* Characteristic: Body sensor location */
                .uuid = BLE_UUID128_DECLARE(SDEVICE_DATA_EVENT),
                .access_cb = gatt_svr_chr_access_event_cb,
                .flags = BLE_GATT_CHR_F_NOTIFY,
            },
            {
                0, /* No more characteristics in this service */
            },
        }
    },

    {
        0, /* No more services */
    },
};

static int
gatt_svr_init(void)
{
    int rc;

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}

//============================================

uint8_t blehr_addr_type;

static bool notify_state;
uint16_t notify_conn_handle;
static const char *device_name = "blehr_sensor";

int
blehr_gap_event(struct ble_gap_event *event, void *arg);
//=======================adv====================
static void
blehr_advertise(void)
{
    ble_qiot_advertising_start();
    return;
}
//==============================================

int
blehr_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        /* A new connection was established or a connection attempt failed */
        MODLOG_DFLT(INFO, "connection %s; status=%d, handle:%x\n",
                    event->connect.status == 0 ? "established" : "failed",
                    event->connect.status, event->connect.conn_handle);

        if (event->connect.status != 0)
        {
            /* Connection failed; resume advertising */
            blehr_advertise();
        }
        else
        {
            ble_gap_connect_cb();
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT:
        MODLOG_DFLT(INFO, "disconnect; reason=%d\n", event->disconnect.reason);

        /* Connection terminated; resume advertising */
        blehr_advertise();
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE:
        MODLOG_DFLT(INFO, "adv complete\n");
        blehr_advertise();
        break;

    case BLE_GAP_EVENT_SUBSCRIBE:
        MODLOG_DFLT(INFO, "subscribe event; cur_notify=%d\n value handle; "
                    "val_handle=%d\n",
                    event->subscribe.cur_notify, event->subscribe.attr_handle);
        if (event->subscribe.attr_handle == 7)
        {
            notify_state = event->subscribe.cur_notify;
            notify_conn_handle = event->subscribe.conn_handle;
        }
        break;

    case BLE_GAP_EVENT_MTU:
        MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d mtu=%d\n",
                    event->mtu.conn_handle,
                    event->mtu.value);
        break;

    }

    return 0;
}

static void
blehr_on_sync(void)
{
    int rc;

    /* Use privacy */
    rc = ble_hs_id_infer_auto(0, &blehr_addr_type);
    assert(rc == 0);

    ble_qiot_explorer_init();
    /* Begin advertising */
    blehr_advertise();

}
// add services and characteristic
void ble_services_add(const qiot_service_init_s *p_service)
{
    int rc;
    static int init_flag = 0;

    //repeat init
    if (init_flag)
        return;
    init_flag = 1;

    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb = blehr_on_sync;

    rc = gatt_svr_init();

    /* startup bluetooth host stack*/
    ble_hs_thread_startup();
}
#ifdef __RTTHREAD__
MSH_CMD_EXPORT_ALIAS(ble_services_add, ble_services_add, "bluetoooth SERVICE senson sample");
#endif
