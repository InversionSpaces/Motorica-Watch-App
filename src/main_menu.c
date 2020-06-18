/*
 * main_menu.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "main_menu.h"

#include "devices.h"
#include "gestures.h"
#include "glist.h"
#include "bt.h"

typedef enum {
	TITLE,
	DEVICES,
	GESTURES,
} item_type_e;

typedef struct {
	item_type_e type;
	appdata_s *ad;
} item_data_s;

static glist_s *menu_list = NULL;

void
push_menu(appdata_s* ad);

void
pop_menu(appdata_s* ad);

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

	switch (type) {
	case DEVICES:
		push_devices(ad);
		break;
	case GESTURES:
		pop_menu(ad);
		push_gestures(ad);
		break;
	default:
		break;
	}
}

static char*
text_get_cb(void *data, Evas_Object *obj, const char *part) {
	item_data_s *item_data = data;
	switch (item_data->type) {
	case DEVICES:
		return strdup("Devices");
	case GESTURES:
		return strdup("Gestures");
	case TITLE:
		return strdup("Motorica");
	default:
		return NULL;
	}
}

void
push_menu(appdata_s* ad) {
	menu_list = glist_create(ad->navif, ad->csurf, clicked_cb, NULL);
	glist_append(menu_list, "title", text_get_cb, item_data_create(TITLE, NULL));
	glist_append(menu_list, "1text", text_get_cb, item_data_create(DEVICES, ad));
	glist_append(menu_list, "1text", text_get_cb, item_data_create(GESTURES, ad));
	glist_append(menu_list, "padding", NULL, NULL);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, menu_list->list, "empty");

	ad->state = MAIN_MENU;
}

void
pop_menu(appdata_s* ad) {
	glist_free(menu_list, item_data_free);
	menu_list = NULL;

	elm_naviframe_item_pop(ad->navif);
}

