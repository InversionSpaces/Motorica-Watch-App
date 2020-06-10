/*
 * main_menu.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "main_menu.h"

#include "gestures.h"
#include "devices.h"
#include "glist.h"
#include "bt.h"

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

void
push_menu(appdata_s* ad) {
	ad->menu_list = glist_create(ad->navif, ad->csurf);

	glist_append_title(ad->menu_list, "Motorica");
	glist_append_entry(ad->menu_list, "Devices", devices_clicked_cb, ad);
	glist_append_entry(ad->menu_list, "Gestures", gestures_clicked_cb, ad);
	glist_append_padding(ad->menu_list);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->menu_list.list, "empty");

	ad->state = MAIN_MENU;
}

