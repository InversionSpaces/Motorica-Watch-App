/*
 * devices.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "devices.h"

#include "glist.h"
#include "bt.h"

static glist_s dev_list = {NULL, NULL};

static enum {
	IDLE,
	DISCOVERING
} discovery_state = IDLE;

typedef struct {
	enum {
		NOT_BONDED,
		UNBONDING,
		BONDING,
		BONDED
	} state;

	char* name;
	char* address;
} dev_s;

static GList *discovered = NULL;

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static char*
refresh_text_get_cb(void *data, Evas_Object *obj, const char *part);

static char*
dev_text_get_cb(void *data, Evas_Object *obj, const char *part);

static void
dev_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e discovery_state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data);

void
bond_created_cb(int result, bt_device_info_s *device_info, void *user_data);

void
bond_destroyed_cb(int result, char* remote_address, void *user_data);

void
dev_s_free(gpointer data);

void
fill_dev_glist();

/*-*/

void
fill_dev_glist() {
	glist_append_title(dev_list, "Devices");
	glist_append(dev_list, "1text", refresh_text_get_cb, NULL, refresh_clicked_cb, NULL);
	glist_append_padding(dev_list);
}

static char*
dev_text_get_cb(void *data, Evas_Object *obj, const char *part) {
	dev_s *info = data;

	if (!strcmp(part, "elm.text")) {
		if (info->name == NULL)
			return NULL;

		return strdup(info->name);
	}
	else if (!strcmp(part, "elm.text.1")) {
		switch (info->state) {
		case BONDED:
			return strdup("Bonded");
		case BONDING:
			return strdup("Bonding...");
		case NOT_BONDED:
			return strdup("Tap to bond");
		case UNBONDING:
			return strdup("Unbonding...");
		}
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "Devices [text_get_cb] unexpected part %s", part);

	return strdup("Unexpected part");
}

static void
dev_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	dev_s *info = data;

	switch (info->state) {
	case BONDED:
		info->state = UNBONDING;
		if (bt_device_destroy_bond(info->address) != BT_ERROR_NONE)
			info->state = BONDED;
		else
			elm_genlist_realized_items_update(dev_list.list);
		break;
	case BONDING:
	case UNBONDING:
		break;
	case NOT_BONDED:
		info->state = BONDING;
		if (bt_device_create_bond(info->address) != BT_ERROR_NONE)
			info->state = NOT_BONDED;
		else
			elm_genlist_realized_items_update(dev_list.list);
		break;
	}
}

static char*
refresh_text_get_cb(void *data, Evas_Object *obj, const char *part) {
	if (!strcmp(part, "elm.text")) {
		if (discovery_state == DISCOVERING)
			return strdup("Refreshing...");
		else if (discovery_state == IDLE)
			return strdup("Refresh");

		dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected discovery state");
		return NULL;
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected part %s", part);

	return NULL;
}

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	if (discovery_state == DISCOVERING) return;

	discovery_state = DISCOVERING;

	if (discovered != NULL) {
		g_list_free_full(discovered, dev_s_free);
		discovered = NULL;

		glist_clear(dev_list);
		fill_dev_glist();
	}

	if (bt_discover(discovery_cb, NULL) != BT_ERROR_NONE)
		discovery_state = IDLE;
	else
		elm_genlist_realized_items_update(dev_list.list);
}

void
dev_s_free(gpointer data) {
	dev_s *info = data;
	free(info->name);
	free(info->address);
	free(info);
}

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data)
{
    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[discovery_cb] failed! result(%d).", result);

        discovery_state = IDLE;
        elm_genlist_realized_items_update(dev_list.list);

        return;
    }

    switch (state) {
    case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_STARTED");
        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FINISHED");

        discovery_state = IDLE;
        elm_genlist_realized_items_update(dev_list.list);

        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FOUND");
        if (discovery_info != NULL) {
            dlog_print(DLOG_INFO, LOG_TAG, "Device Address: %s", discovery_info->remote_address);
            dlog_print(DLOG_INFO, LOG_TAG, "Device Name is: %s", discovery_info->remote_name);

            dev_s *info = malloc(sizeof(dev_s));
            if (info == NULL) {
            	dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
            	return;
            }

			info->address = strdup(discovery_info->remote_address);
			info->name = strdup(discovery_info->remote_name);
			info->state = discovery_info->is_bonded ? BONDED : NOT_BONDED;
			discovered = g_list_append(discovered, (gpointer)info);

			glist_insert_after_first(
					dev_list, "2text", dev_text_get_cb, info, dev_clicked_cb, info
				);
        }
        break;
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
    	for (GList* it = discovered; it != NULL; it = it->next) {
    		dev_s* info = it->data;
    		if (!strcmp(info->address, device_info->remote_address)) {
    			info->state = BONDED;
    			dlog_print(DLOG_ERROR, LOG_TAG, "%s is bonded", info->name);
    		}
    	}

    	elm_genlist_realized_items_update(dev_list.list);
    }
}

void
bond_destroyed_cb(int result, char* remote_address, void *user_data) {
	 if (result != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[bond_destroyed_cb] failed. result(%d).", result);

		return;
	}

	if (remote_address != NULL) {
		for (GList* it = discovered; it != NULL; it = it->next) {
			dev_s* info = it->data;
			if (!strcmp(info->address, remote_address)) {
				info->state = NOT_BONDED;
				dlog_print(DLOG_ERROR, LOG_TAG, "%s is not bonded", info->name);
			}
		}

		elm_genlist_realized_items_update(dev_list.list);
	}
}

void push_devices(appdata_s* ad) {
	bt_adapter_set_device_discovery_state_changed_cb(discovery_cb, NULL);
	bt_device_set_bond_created_cb(bond_created_cb, NULL);
	bt_device_set_bond_destroyed_cb(bond_destroyed_cb, NULL);

	if (discovered != NULL) {
			g_list_free_full(discovered, dev_s_free);
			discovered = NULL;
	}

	dev_list = glist_create(ad->navif, ad->csurf);
	fill_dev_glist();

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, dev_list.list, "empty");

	ad->state = DEVICES;
}


