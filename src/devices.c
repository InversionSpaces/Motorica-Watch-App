/*
 * devices.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "devices.h"

#include "glist.h"
#include "bt.h"

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static void
fill_dev_list(appdata_s *ad) {
	glist_append_title(ad->dev_list, "Devices");
	glist_append_entry(ad->dev_list, "Refresh", refresh_clicked_cb, ad);
	glist_append_padding(ad->dev_list);
}

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part) {
	bt_adapter_device_discovery_info_s *info = data;

	if (!strcmp(part, "elm.text")) {
		if (info->remote_name == NULL)
			return NULL;

		return strdup(info->remote_name);
	}

	return strdup("Tap to connect");
}

void
info_free(gpointer data) {
	bt_adapter_device_discovery_info_s *info = data;
	free(info->remote_name);
	free(info->remote_address);
	free(info);
}

static void
discovery_cb(int result, bt_adapter_device_discovery_state_e discovery_state,
             bt_adapter_device_discovery_info_s *discovery_info, void* user_data)
{
	appdata_s *ad = user_data;

    if (result != BT_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "[adapter_device_discovery_state_changed_cb] failed! result(%d).", result);

        ad->discovery_state = IDLE;

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

            bt_adapter_device_discovery_info_s *info = malloc(sizeof(bt_adapter_device_discovery_info_s));
            if (info == NULL) {
            	dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
            	return;
            }

			memcpy(info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
			info->remote_address = strdup(discovery_info->remote_address);
			info->remote_name = strdup(discovery_info->remote_name);
			ad->discovered = g_list_append(ad->discovered, (gpointer)info);

			Elm_Object_Item* item =
					glist_insert_after_first(ad->dev_list, "2text", text_get_cb, info, NULL, NULL);
			elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
        }
        break;
    }
}

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	dlog_print(DLOG_INFO, LOG_TAG, "Refresh clicked");

	appdata_s *ad = data;

	if (ad->discovery_state == IDLE) {
		if (ad->discovered != NULL) {
			g_list_free_full(ad->discovered, info_free);
			ad->discovered = NULL;

			glist_clear(ad->dev_list);
			fill_dev_list(ad);
		}

		bt_discover(discovery_cb, ad);
	}
}

void push_devices(appdata_s* ad) {
	ad->dev_list = glist_create(ad->navif, ad->csurf);
	fill_dev_list(ad);

	ad->discovered = NULL;

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->dev_list.list, "empty");

	ad->state = DEVICES;
}


