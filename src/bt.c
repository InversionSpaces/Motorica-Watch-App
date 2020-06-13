/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */
#include <app_control.h>

#include "bt.h"

void
bt_init(void) {
	bt_error_e ret = bt_initialize();

	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");
}

void
bt_deinit(void) {
	bt_error_e ret = bt_deinitialize();

	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_deinitialize] failed.");
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
bt_discover() {
	bt_error_e ret = bt_adapter_start_device_discovery();

	if (ret != BT_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_start_device_discovery] failed.");
	else
		dlog_print(DLOG_INFO, LOG_TAG, "[bt_adapter_start_device_discovery] didn't fail.");

	return ret;
}



