/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */
#include "bt.h"

#include <bluetooth.h>
#include <app_control.h>

static void
socket_connection_state_changed(int result, bt_socket_connection_state_e connection_state,
                                bt_socket_connection_s *connection, void *user_data)
{
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[socket_connection_state_changed_cb] failed. result =%d.", result);

        return;
    }

    if (connection_state == BT_SOCKET_CONNECTED) {
        dlog_print(DLOG_INFO, LOG_TAG, "Connection: Connected.");
        if (connection != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of connection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Role of connection - %d.", connection->local_role);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);
        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "Connection: Disconnected.");
        if (connection != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Socket of disconnection - %d.", connection->socket_fd);
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: Address of connection - %s.", connection->remote_address);
        } else {
            dlog_print(DLOG_INFO, LOG_TAG, "Callback: No connection data");
        }
    }
}

void
bt_init(appdata_s *ad) {
	bt_error_e ret = bt_initialize();

	if (ret != BT_ERROR_NONE) {
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_initialize] failed.");

	    return;
	}

	// Set connection changed cb
	ret = bt_socket_set_connection_state_changed_cb(socket_connection_state_changed, ad);
	if (ret != BT_ERROR_NONE) {
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_set_connection_state_changed_cb] failed.");

	    return;
	}
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
		dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_start_device_discovery] failed with result (%d).", ret);

	return ret;
}



