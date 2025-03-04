/*
 * connect_devices.c
 *
 *  Created on: Jun 18, 2020
 *      Author: inv
 */

#include <bluetooth.h>

#include "connect_devices.h"
#include "glist.h"
#include "bt.h"

/*
 * Menu item type enum
 */
typedef enum {
	TITLE,
	DEVICE
} item_type_e;

/*
 * Device info struct
 */
typedef struct {
	char* name;
	char* address;
} dev_s;

/*
 * Menu item data struct
 */
typedef struct {
	item_type_e type;
	dev_s *info;
	appdata_s *ad;
} item_data_s;

/*
 * Devices general list
 */
static glist_s *devices_list = NULL;

/*
 * Text get callback for general list
 */
static char*
text_get_cb(void *data, Evas_Object *obj, const char *part);

/*
 * Clicked callback for general list
 */
static void
clicked_cb(void *data, Evas_Object *obj, void *event_info);

/*
 * Callback called for each bonded device
 */
static bool
bonded_device_cb(bt_device_info_s *device_info, void *user_data);

/*-*/

static dev_s*
dev_info_create(const char* name, const char* address) {
	dev_s *info = malloc(sizeof(dev_s));
	if (info == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
		return NULL;
	}

	info->address = strdup(address);
	info->name = strdup(name);

	return info;
}

static void
dev_info_free(void *data) {
	if (data == NULL) return;
	dev_s *info = data;

	free(info->address);
	free(info->name);
	free(info);
}

static item_data_s*
item_data_create(item_type_e type, appdata_s *ad, dev_s *info) {
	item_data_s* retval = malloc(sizeof(item_data_s));
	if (retval == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
		return NULL;
	}

	retval->type = type;
	retval->info = info;
	retval->ad = ad;

	return retval;
}

static void
item_data_free(void *data) {
	if (data == NULL) return;

	item_data_s *item_data = data;
	dev_info_free(item_data->info);
	free(data);
}

static void
clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	Elm_Object_Item *it = event_info;
	item_data_s *item_data = elm_object_item_data_get(it);

	// For padding data can be NULL
	if (item_data == NULL) return;

	item_type_e type = item_data->type;
	dev_s *info = item_data->info;

	// Nothing to do
	if (type != DEVICE) return;

	// Try to connect
	bt_error_e ret = bt_socket_connect_rfcomm(info->address, "00001101-0000-1000-8000-00805F9B34FB");
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_socket_connect_rfcomm] failed.");
}

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part) {
	item_data_s *item_data = data;
	dev_s *info = item_data->info;

	// Dispatch
	switch (item_data->type) {
	case DEVICE:
		return strdup(info->name);
	case TITLE:
		return strdup("Connect devices");
	default:
		return NULL;
	}
}

static bool
bonded_device_cb(bt_device_info_s *device_info, void *user_data)
{
    if (device_info == NULL)
        return true;

    appdata_s *ad = user_data;

    dev_s *info = dev_info_create(device_info->remote_name, device_info->remote_address);
    if (info == NULL) {
    	dlog_print(DLOG_ERROR, LOG_TAG, "[dev_info_create] failed");
    	return false;
    }

    item_data_s *item_data = item_data_create(DEVICE, ad, info);
    if (item_data == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[item_data_create] failed");

		// Free allocated info
		dev_info_free(info);

		return false;
	}

    // Insert item to devices list
	glist_insert_after_first(
			devices_list, "1text", text_get_cb, item_data
	);

    return true;
}

void
push_connect_devices(appdata_s* ad) {
	// Creting general list
	devices_list = glist_create(ad->navif, ad->csurf, clicked_cb, NULL);

	glist_append(devices_list, "title", text_get_cb, item_data_create(TITLE, NULL, NULL));
	glist_append(devices_list, "padding", NULL, NULL);

	// Iterating bonded devices
	bt_error_e ret = bt_adapter_foreach_bonded_device(bonded_device_cb, ad);
	if (ret != BT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_foreach_bonded_device] failed!");

	// Pushing list to stack
	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, devices_list->list, "empty");

	// Update app state
	ad->state = CONNECT_DEVICES;
}

void
pop_connect_devices(appdata_s* ad) {
	// Deleting list
	glist_free(devices_list, item_data_free);
	devices_list = NULL;

	// Poping from naviframe stack - supposing we are on tops
	elm_naviframe_item_pop(ad->navif);
}


