/*
 * devices.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "devices.h"

static glist_entry_s entries[] = {
		{"Devices", "title", NULL},
		{"Refresh", "entry", NULL},
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


