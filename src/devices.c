/*
 * devices.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "devices.h"

static void
refresh_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	dlog_print(DLOG_INFO, LOG_TAG, "Refresh clicked");

	appdata_s* ad = data;

	if (ad->discovery_state == IDLE)
		bt_discover(ad);
}

static glist_entry_s entries[] = {
		{"Devices", "title", NULL},
		{"Refresh", "entry", refresh_clicked_cb},
		{NULL, "padding", NULL}
};

void push_devices(appdata_s* ad) {
	size_t size = sizeof(entries) / sizeof(glist_entry_s);
	lists_s ret = glist_create(ad, entries, size);
	ad->devices_glist = ret.glist;
	ad->devices_circle_glist = ret.circle_glist;

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->devices_glist, "empty");

	ad->state = DEVICES;
}


