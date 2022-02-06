from building import *
import rtconfig

cwd = GetCurrentDir()
path = [cwd]
src = []

path += [
    cwd + '/date_template',
    cwd + '/qcloud_iot_explorer_ble/inc',
    cwd + '/qcloud_iot_explorer_ble/src/internal_inc']

src += Split('''
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_base64.c
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_crc.c
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_hmac.c
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_log.c
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_md5.c
    qcloud_iot_explorer_ble/src/utils/ble_qiot_utils_sha1.c
    ''')

src += Split('''
    date_template/ble_qiot_template.c
    ''')

src += Split('''
    qcloud_iot_explorer_ble/src/core/ble_qiot_llsync_data.c
    qcloud_iot_explorer_ble/src/core/ble_qiot_llsync_device.c
    qcloud_iot_explorer_ble/src/core/ble_qiot_llsync_event.c
    qcloud_iot_explorer_ble/src/core/ble_qiot_llsync_ota.c
    qcloud_iot_explorer_ble/src/core/ble_qiot_service.c
    ''')


path += [
    cwd + '/hal/nimble']
    
    
src += Split('''
    hal/nimble/ble_qiot_ble_device.c
    hal/nimble/flash_storage.c    
    hal/nimble/mcu_service.c   
    ''')


LOCAL_CCFLAGS = ''

if rtconfig.CROSS_TOOL == 'keil':
    LOCAL_CCFLAGS += ' --gnu '
    
group = DefineGroup('llsync', src, depend = ['PKG_USING_NIMBLE'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
