/*
 * main_menu.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "main_menu.h"

#define SIZE(x) sizeof(x) / sizeof(0[x])

void
gestures_clicked_cb(void *data, Evas_Object *obj, void *event_info) {

}

typedef struct {
	const char* text;
	const char* style;
	Evas_Smart_Cb clicked_cb;
} mentry_s;

mentry_s mentries[] = {
		{"Motorica", "title", NULL},
		{"Devices", "entry", NULL},
		{"Gestures", "entry", gestures_clicked_cb},
		{NULL, "padding", NULL}
};

static char*
mentry_text_get(void *data, Evas_Object *obj, const char *part) {
	int index = (int)data;
	if (index < 0 || index >= SIZE(mentries)) {
		dlog_print(DLOG_ERROR, LOG_TAG, "index out of bounds");
		return NULL;
	}

	const char* text = mentries[index].text;

	if (text == NULL) return NULL;
	return strdup(text);
}

Elm_Genlist_Item_Class*
glist_itc_fill(Elm_Genlist_Item_Class *itc, const char *style) {
	if (itc == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "itc is NULL");
		return NULL;
	}
	if (style == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "style is NULL");
		return NULL;
	}

	if (!strcmp(style, "title")) {
		itc->item_style = "title";
		itc->func.text_get = mentry_text_get;
	}
	else if (!strcmp(style, "entry")) {
		itc->item_style = "1text";
		itc->func.text_get = mentry_text_get;
	}
	else if (!strcmp(style, "padding")) {
		/* nothing to do */
	}
	else {
		dlog_print(DLOG_ERROR, LOG_TAG, "Unknown style %s", style);
	}

	return itc;
}

Elm_Object_Item*
glist_append(Evas_Object *glist, const char *style, const void *data,
		Evas_Smart_Cb clicked_cb, const void *cb_data) {
	if (glist == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "glist is NULL");
		return NULL;
	}

	if (style == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "style is NULL");
		return NULL;
	}

	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
	glist_itc_fill(itc, style);
	Elm_Object_Item* item = elm_genlist_item_append(glist, itc, data, NULL, ELM_GENLIST_ITEM_NONE, clicked_cb, cb_data);
	elm_genlist_item_class_free(itc);

	return item;
}

void
push_menu(appdata_s* ad) {
	Evas_Object *gen_list = elm_genlist_add(ad->navif);
	Evas_Object *cgen_list = eext_circle_object_genlist_add(gen_list, ad->csurf);

	eext_circle_object_genlist_scroller_policy_set(cgen_list, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(cgen_list, EINA_TRUE);

	/*
	//
	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

	itc->item_style = "title";
	itc->func.text_get = data_get_title_text;

	elm_genlist_item_append(gen_list, itc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_class_free(itc);
	//

	//
	itc = elm_genlist_item_class_new();

	itc->item_style = "1text";
	itc->func.text_get = data_get_entry_text;

	elm_genlist_item_append(gen_list, itc, (void*)1, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_class_free(itc);
	//

	//
	itc = elm_genlist_item_class_new();

	itc->item_style = "1text";
	itc->func.text_get = data_get_entry_text;

	elm_genlist_item_append(gen_list, itc, (void*)2, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_class_free(itc);
	//

	//
	itc = elm_genlist_item_class_new();

	elm_genlist_item_append(gen_list, itc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_class_free(itc);
	//
	*/

	for (int i = 0; i < SIZE(mentries); ++i) {
		mentry_s* entry = mentries + i;
		glist_append(gen_list, entry->style, (void*)i, entry->clicked_cb, (void*)i);
	}

	evas_object_show(gen_list);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, gen_list, "empty");
}

