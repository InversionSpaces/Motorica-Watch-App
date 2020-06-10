/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "bt.h"

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e discovery_state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data)
{
	appdata_s* ad = user_data;

    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[adapter_device_discovery_state_changed_cb] failed! result(%d).", result);

        return;
    }

    switch (discovery_state) {
    case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
    	ad->discovery_state = DISCOVERING;
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_STARTED");
        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
    	ad->discovery_state = IDLE;
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FINISHED");
        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FOUND");
        if (discovery_info != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Device Address: %s", discovery_info->remote_address);
            dlog_print(DLOG_INFO, LOG_TAG, "Device Name is: %s", discovery_info->remote_name);
        }
        break;
    }
}

void
bt_init(appdata_s* ad) {
	bt_error_e ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");
	    return;
	}

	ret = bt_adapter_set_device_discovery_state_changed_cb(discovery_cb, ad);
	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG,
				"[bt_adapter_set_device_discovery_state_changed_cb] failed.");
	else
		dlog_print(DLOG_INFO, LOG_TAG,
				"[bt_initialize&bt_adapter_set_device_discovery_state_changed_cb] didn't fail.");

	ad->discovery_state = IDLE;
}

int
bt_onoff_operation(void) {
    app_control_h service = NULL;
    app_control_create(&service);

    if (service == NULL) {
        dlog_print(DLOG_INFO, LOG_TAG, "service_create failed!\n");

        return 0;
    }

    app_control_set_operation(service, APP_CONTROL_OPERATION_SETTING_BT_ENABLE);
    int ret = app_control_send_launch_request(service, NULL, NULL);
    app_control_destroy(service);

    if (ret == APP_CONTROL_ERROR_NONE) {
        dlog_print(DLOG_INFO, LOG_TAG, "Succeeded to Bluetooth On/Off app!\n");

        return 0;
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "Failed to relaunch Bluetooth On/Off app!\n");

        return -1;
    }

    return 0;
}

bool
bt_is_on(void) {
	bt_adapter_state_e adapter_state;

	bt_error_e ret = bt_adapter_get_state(&adapter_state);
	if (ret != BT_ERROR_NONE) {
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_get_state] failed");

	    return false;
	}

	if (adapter_state == BT_ADAPTER_DISABLED)
	    return false;

	return true;
}


int
bt_discover(appdata_s* ad) {
	bt_error_e ret = bt_adapter_start_device_discovery();

	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_start_device_discovery] failed.");
		ad->discovery_state = STARTING;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "[bt_adapter_start_device_discovery] didn't fail.");

	return ret;
}



