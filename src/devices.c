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

void push_devices(appdata_s* ad) {
	ad->dev_list = glist_create(ad->navif, ad->csurf);
	glist_append_title(ad->dev_list, "Devices");
	glist_append_entry(ad->dev_list, "Refresh", refresh_clicked_cb, ad);
	glist_append_padding(ad->dev_list);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->dev_list.list, "empty");

	ad->state = DEVICES;
}


