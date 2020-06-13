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
info_free(gpointer data);

void
fill_dev_glist(appdata_s *ad) {
	glist_append_title(ad->dev_list, "Devices");
	glist_append(ad->dev_list, "1text", refresh_text_get_cb, ad, refresh_clicked_cb, ad);
	glist_append_padding(ad->dev_list);
}

static char*
dev_text_get_cb(void *data, Evas_Object *obj, const char *part) {
	bt_adapter_device_discovery_info_s *info = data;

	if (!strcmp(part, "elm.text")) {
		if (info->remote_name == NULL)
			return NULL;

		return strdup(info->remote_name);
	}
	else if (!strcmp(part, "elm.text.1")) {
		if (info->is_bonded)
			return strdup("Bonded. Tap to connect.");
		else
			return strdup("Tap to bond");
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "Devices [text_get_cb] unexpected part %s", part);

	return strdup("Unexpected part");
}

static void
dev_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	bt_adapter_device_discovery_info_s *info = data;

	dlog_print(DLOG_INFO, LOG_TAG, "%s clicked", info->remote_name);
}

static char*
refresh_text_get_cb(void *data, Evas_Object *obj, const char *part) {
	appdata_s *ad = data;

	if (!strcmp(part, "elm.text")) {
		if (ad->discovery_state == DISCOVERING)
			return strdup("Refreshing...");
		else if (ad->discovery_state == IDLE)
			return strdup("Refresh");
		else {
			dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected discovery state");

			return strdup("Unexpected discovery state");
		}
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[refresh text_get_cb] unexpected part %s", part);

	return strdup("Unexpected part");
}

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	appdata_s *ad = data;

	if (ad->discovery_state == DISCOVERING) return;

	ad->discovery_state = DISCOVERING;

	if (ad->discovered != NULL) {
		g_list_free_full(ad->discovered, info_free);
		ad->discovered = NULL;

		glist_clear(ad->dev_list);
		fill_dev_glist(ad);
	}

	if (bt_discover(discovery_cb, ad) != BT_ERROR_NONE)
		ad->discovery_state = IDLE;

	elm_genlist_realized_items_update(ad->dev_list.list);
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
        elm_genlist_realized_items_update(ad->dev_list.list);

        return;
    }

    switch (discovery_state) {
    case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_STARTED");
        break;
    case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
        dlog_print(DLOG_INFO, LOG_TAG, "BT_ADAPTER_DEVICE_DISCOVERY_FINISHED");

        ad->discovery_state = IDLE;
        elm_genlist_realized_items_update(ad->dev_list.list);

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

			glist_insert_after_first(
					ad->dev_list, "2text", dev_text_get_cb, info, dev_clicked_cb, info
				);
        }
        break;
    }
}

void push_devices(appdata_s* ad) {
	ad->dev_list = glist_create(ad->navif, ad->csurf);
	fill_dev_glist(ad);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->dev_list.list, "empty");

	ad->state = DEVICES;
}


