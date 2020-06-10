/*
 * main_menu.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "main_menu.h"

static void
gestures_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	appdata_s *ad = data;

	elm_naviframe_item_pop(ad->navif);
	push_gestures(ad);
}

static void
devices_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	appdata_s *ad = data;

	if (bt_is_on()) {
		elm_naviframe_item_pop(ad->navif);
		push_devices(ad);
	}
	else bt_onoff_operation();
}

static glist_entry_s entries[] = {
		{"Motorica", "title", NULL},
		{"Devices", "entry", devices_clicked_cb},
		{"Gestures", "entry", gestures_clicked_cb},
		{NULL, "padding", NULL}
};

void
push_menu(appdata_s* ad) {
	size_t size = sizeof(entries) / sizeof(glist_entry_s);
	lists_s ret = glist_create(ad, entries, size);
	ad->glist = ret.glist;
	ad->circle_glist = ret.circle_glist;

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->glist, "empty");

	ad->state = MAIN_MENU;
}

