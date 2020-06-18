/*
 * devices.c
 *
 *  Created on: Jun 18, 2020
 *      Author: inv
 */

#include "devices.h"

#include "glist.h"
#include "bond_devices.h"
#include "connect_devices.h"
#include "bt.h"

typedef enum {
	TITLE,
	CONNECT,
	BOND,
} item_type_e;

typedef struct {
	item_type_e type;
	appdata_s *ad;
} item_data_s;

static glist_s *devices_list = NULL;

void
push_devices(appdata_s* ad);

void
pop_devices(appdata_s* ad);

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part);

static void
clicked_cb(void *data, Evas_Object *obj, void *event_info);

static item_data_s*
item_data_create(item_type_e type, appdata_s* ad);

static void
item_data_free(void *data);

/*-*/

static item_data_s*
item_data_create(item_type_e type, appdata_s* ad) {
	item_data_s* retval = malloc(sizeof(item_data_s));
	if (retval == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "[malloc] failed");
		return NULL;
	}

	retval->type = type;
	retval->ad = ad;

	return retval;
}

static void
item_data_free(void *data) {
	if (data == NULL) return;
	free(data);
}

static void
clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	Elm_Object_Item *it = event_info;
	item_data_s *item_data = elm_object_item_data_get(it);

	item_type_e type = item_data->type;
	appdata_s *ad = item_data->ad;

	if (!bt_is_on()) {
		bt_onoff_operation();
		return;
	}

	switch (type) {
	case CONNECT:
		pop_devices(ad);
		push_connect_devices(ad);
		break;
	case BOND:
		pop_devices(ad);
		push_bond_devices(ad);
		break;
	default:
		break;
	}
}

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part) {
	item_data_s *item_data = data;
	switch (item_data->type) {
	case CONNECT:
		return strdup("Connect to bonded");
	case BOND:
		return strdup("Bond to discovered");
	case TITLE:
		return strdup("Devices");
	default:
		return NULL;
	}
}

void
push_devices(appdata_s* ad) {
	devices_list = glist_create(ad->navif, ad->csurf, clicked_cb, NULL);
	glist_append(devices_list, "title", text_get_cb, item_data_create(TITLE, NULL));
	glist_append(devices_list, "1text", text_get_cb, item_data_create(CONNECT, ad));
	glist_append(devices_list, "1text", text_get_cb, item_data_create(BOND, ad));
	glist_append(devices_list, "padding", NULL, NULL);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, devices_list->list, "empty");

	ad->state = DEVICES;
}

void
pop_devices(appdata_s* ad) {
	glist_free(devices_list, item_data_free);
	devices_list = NULL;

	elm_naviframe_item_pop(ad->navif);
}


