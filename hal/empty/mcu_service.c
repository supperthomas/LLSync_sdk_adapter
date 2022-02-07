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

#include "rtthread.h"
#include <stdint.h>
#include <string.h>

#include "ble_qiot_service.h"
#include "ble_qiot_import.h"
#include "host/ble_hs.h"
#include "host/ble_uuid.h"


#include "ble_qiot_export.h"

//IOT_BLE_UUID_BASE + IOT_BLE_UUID_SERVICE
#define SERVICE_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE0, 0xFF, 0x00, 0x00 
//    
   

//IOT_BLE_UUID_BASE + IOT_BLE_UUID_DEVICE_INFO
#define SDEVICE_INFO_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE1, 0xFF, 0x00, 0x00 
    
    
//IOT_BLE_UUID_BASE + IOT_BLE_UUID_DATA
#define SDEVICE_DATA_BASE                                                                              \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE2, 0xFF, 0x00, 0x00 
//IOT_BLE_UUID_BASE + IOT_BLE_UUID_EVENT
#define SDEVICE_DATA_EVENT                                                                           \
        0xe2, 0xa4, 0x1b, 0x54, 0x93, 0xe4, 0x6a, 0xb5, 0x20, 0x4e, 0xd0, 0x65, 0xE3, 0xFF, 0x00, 0x00 
        

static int
gatt_svr_chr_access_device_info(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */

    return 0;
}

static int
gatt_svr_chr_access_device_data(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */
    return 0;
}
static int
gatt_svr_chr_access_event_cb(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */
  
    return 0;
}

static uint16_t device_info_handle;
static uint16_t device_data_handle;
static uint16_t device_event_handle;
static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /* Service: Heart-rate */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID128_DECLARE(SERVICE_BASE),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            /* Characteristic: Heart-rate measurement */
            .uuid = BLE_UUID128_DECLARE(SDEVICE_INFO_BASE),
           .access_cb = gatt_svr_chr_access_device_info,
            .val_handle = &device_info_handle,
            .flags = BLE_GATT_CHR_F_WRITE,
        }, {
            /* Characteristic: Body sensor location */
            .uuid = BLE_UUID128_DECLARE(SDEVICE_DATA_BASE),
           .access_cb = gatt_svr_chr_access_device_data,
            .flags = BLE_GATT_CHR_F_WRITE,
        }, {
            /* Characteristic: Body sensor location */
            .uuid = BLE_UUID128_DECLARE(SDEVICE_DATA_EVENT),
           .access_cb = gatt_svr_chr_access_event_cb,
            .val_handle = &device_event_handle,
            .flags = BLE_GATT_CHR_F_NOTIFY,
        }, {
            0, /* No more characteristics in this service */
        }, }
    },

        {
            0, /* No more services */
        },
};

// add services and characteristic
void ble_services_add(const qiot_service_init_s *p_service)
{
    int rc;
    static int init_flag = 0;

    if (init_flag)
        return ;
    init_flag = 1;

//    /* Initialize the NimBLE host configuration */
//    ble_hs_cfg.sync_cb = blehr_on_sync;

//    ble_npl_callout_init(&blehr_tx_timer, nimble_port_get_dflt_eventq(),
//                    blehr_tx_hrate, NULL);

//    rc = ble_gatts_count_cfg(gatt_svr_svcs);
//    if (rc != 0) {
//        return;
//    }

//    rc = ble_gatts_add_svcs(gatt_svr_svcs);
//    if (rc != 0) {
//        return;
//    }

//    /* Set the default device name */
//    rc = ble_svc_gap_device_name_set(device_name);
//    RT_ASSERT(rc == 0);

//    /* startup bluetooth host stack*/
//    ble_hs_thread_startup();
    
}

MSH_CMD_EXPORT_ALIAS(ble_services_add, ble_services_add, "bluetoooth SERVICE senson sample");