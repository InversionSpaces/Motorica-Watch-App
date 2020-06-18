/*
 * devices.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include <bluetooth.h>

#include "bond_devices.h"
#include "glist.h"
#include "bt.h"

typedef enum {
	IDLE,
	DISCOVERING
} disc_state_e;

typedef struct {
	bool is_bonded;
	char* name;
	char* address;
} dev_s;

typedef enum {
	TITLE,
	DEVICE,
	REFRESH,
} item_type_e;

typedef struct {
	item_type_e type;
	dev_s *info;
} item_data_s;

static glist_s *dev_list = NULL;

static disc_state_e disc_state = IDLE;

/*-*/

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part);

static void
clicked_cb(void *data, Evas_Object *obj, void *event_info);

static char*
refresh_text_get_cb(const char *part);

static void
refresh_clicked_cb();

static char*
dev_text_get_cb(dev_s *info, const char *part);

static void
dev_clicked_cb(dev_s *info);

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e discovery_state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data);

void
bond_created_cb(int result, bt_device_info_s *device_info, void *user_data);

void
bond_destroyed_cb(int result, char* remote_address, void *user_data);

/*-*/

static dev_s*
dev_info_create(const char *address, const char *name, bool is_bonded) {
	dev_s *info = malloc(sizeof(dev_s));
	if (info == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
		return NULL;
	}

	info->address = strdup(address);
	info->name = strdup(name);
	info->is_bonded = is_bonded;

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
item_data_create(item_type_e type, dev_s *info) {
	item_data_s* retval = malloc(sizeof(item_data_s));
	if (retval == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
		return NULL;
	}

	retval->type = type;
	retval->info = info;

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
fill_dev_list() {
	glist_append(dev_list, "title", text_get_cb, item_data_create(TITLE, NULL));
	glist_append(dev_list, "1text", text_get_cb, item_data_create(REFRESH, NULL));
	glist_append(dev_list, "padding", NULL, NULL);
}

static void
clear_dev_list() {
	glist_clear(dev_list, item_data_free);
}

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part) {
	item_data_s *item_data = data;

	switch (item_data->type) {
	case TITLE:
		return strdup("Bond devices");
	case DEVICE:
		return dev_text_get_cb(item_data->info, part);
	case REFRESH:
		return refresh_text_get_cb(part);
	default:
		return NULL;
	}
}

void
clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	Elm_Object_Item *it = event_info;
	item_data_s *item_data = elm_object_item_data_get(it);

	if (item_data == NULL) return;

	switch (item_data->type) {
	case DEVICE:
		dev_clicked_cb(item_data->info);
		break;
	case REFRESH:
		refresh_clicked_cb();
		break;
	default:
		break;
	}
}

static char*
dev_text_get_cb(dev_s *info, const char *part) {
	if (!strcmp(part, "elm.text")) {
		if (info->name == NULL)
			return NULL;

		return strdup(info->name);
	}
	else if (!strcmp(part, "elm.text.1")) {
		if (info->is_bonded)
			return strdup("Bonded");
		return strdup("Tap to bond");
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[dev_text_get_cb] unexpected part %s", part);

	return NULL;
}

static void
dev_clicked_cb(dev_s *info) {
	if (info->is_bonded) {
		bt_error_e ret = bt_device_destroy_bond(info->address);
		if (ret != BT_ERROR_NONE)
			dlog_print(DLOG_ERROR, LOG_TAG, "[bt_device_destroy_bond] failed");
	}
	else {
		bt_error_e ret = bt_device_create_bond(info->address);
		if (ret != BT_ERROR_NONE)
			dlog_print(DLOG_ERROR, LOG_TAG, "[bt_device_create_bond] failed");
	}
}

static char*
refresh_text_get_cb(const char *part) {
	if (!strcmp(part, "elm.text")) {
		if (disc_state == DISCOVERING)
			return strdup("Refreshing...");
		else if (disc_state == IDLE)
			return strdup("Refresh");

		dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected discovery state");

		return NULL;
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected part %s", part);

	return NULL;
}

static void
refresh_clicked_cb() {
	if (disc_state == DISCOVERING) return;

	disc_state = DISCOVERING;

	clear_dev_list();
	fill_dev_list();

	if (bt_discover() != BT_ERROR_NONE)
		disc_state = IDLE;
	else
		elm_genlist_realized_items_update(dev_list->list);
}

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data)
{
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[discovery_cb] failed! result(%d).", result);

        disc_state = IDLE;
        elm_genlist_realized_items_update(dev_list->list);

        return;
    }

    switch (state) {
    case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_STARTED");
        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FINISHED");

        disc_state = IDLE;
        elm_genlist_realized_items_update(dev_list->list);

        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FOUND");
        if (discovery_info != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Device Address: %s", discovery_info->remote_address);
            dlog_print(DLOG_INFO, LOG_TAG, "Device Name is: %s", discovery_info->remote_name);

            dev_s *info = dev_info_create(
					discovery_info->remote_address,
					discovery_info->remote_name,
					discovery_info->is_bonded
            );
			if (info == NULL) {
				dlog_print(DLOG_ERROR, LOG_TAG, "[dev_info_create] failed");
				return;
			}

			item_data_s *item_data = item_data_create(DEVICE, info);
			if (item_data == NULL) {
				dlog_print(DLOG_ERROR, LOG_TAG, "[item_data_create] failed");
				dev_info_free(info);
				return;
			}

			glist_insert_after_first(
				dev_list, "2text", text_get_cb, item_data
			);
        }
        break;
    }
}

void set_bonded(const char* address, bool is_bonded) {
	for (	Elm_Widget_Item *it = elm_genlist_first_item_get(dev_list->list);
			it != NULL;
			it = elm_genlist_item_next_get(it)) {
		item_data_s *item_data = elm_object_item_data_get(it);
		if (item_data == NULL || item_data->type != DEVICE) continue;

		dev_s *info = item_data->info;
		if (!strcmp(info->address, address)) {
			info->is_bonded = is_bonded;
			dlog_print(DLOG_ERROR, LOG_TAG, "%s is bonded", info->name);
		}
	}
}

void
bond_created_cb(int result, bt_device_info_s *device_info, void *user_data)
{
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[bond_created_cb] failed. result(%d).", result);

        return;
    }

    if (device_info != NULL) {
    	set_bonded(device_info->remote_address, true);

    	elm_genlist_realized_items_update(dev_list->list);
    }
}

void
bond_destroyed_cb(int result, char* remote_address, void *user_data) {
	 if (result != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bond_destroyed_cb] failed. result(%d).", result);

		return;
	}

	if (remote_address != NULL) {
		set_bonded(remote_address, false);

		elm_genlist_realized_items_update(dev_list->list);
	}
}

void push_bond_devices(appdata_s* ad) {
	dev_list = glist_create(ad->navif, ad->csurf, clicked_cb, NULL);
	fill_dev_list();

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, dev_list->list, "empty");

	bt_adapter_set_device_discovery_state_changed_cb(discovery_cb, NULL);
	bt_device_set_bond_created_cb(bond_created_cb, NULL);
	bt_device_set_bond_destroyed_cb(bond_destroyed_cb, NULL);

	ad->state = BOND_DEVICES;
}

void pop_bond_devices(appdata_s* ad) {
	bt_adapter_unset_device_discovery_state_changed_cb();
	bt_device_unset_bond_created_cb();
	bt_device_unset_bond_destroyed_cb();

	bt_adapter_stop_device_discovery();
	bt_device_cancel_bonding();

	glist_free(dev_list, item_data_free);
	dev_list = NULL;
	disc_state = IDLE;
}


