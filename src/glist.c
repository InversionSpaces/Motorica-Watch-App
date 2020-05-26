/*
 * glist.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include "glist.h"

static char*
glist_entry_text_get(void *data, Evas_Object *obj, const char *part) {
	const char* text = data;

	if (text == NULL) return NULL;
	return strdup(text);
}

static Elm_Genlist_Item_Class *
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
		itc->func.text_get = glist_entry_text_get;
	}
	else if (!strcmp(style, "entry")) {
		itc->item_style = "1text";
		itc->func.text_get = glist_entry_text_get;
	}
	else if (!strcmp(style, "padding")) {
		/* nothing to do */
	}
	else {
		dlog_print(DLOG_ERROR, LOG_TAG, "Unknown style %s", style);
	}

	return itc;
}

static Elm_Object_Item *
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
	Elm_Object_Item* item = elm_genlist_item_append(
				glist, itc, data, NULL,
				ELM_GENLIST_ITEM_NONE, clicked_cb, cb_data
				);
	elm_genlist_item_class_free(itc);

	return item;
}

static Evas_Object *
circle_glist_create(appdata_s *ad, Evas_Object *glist) {
	Evas_Object *circle_glist = eext_circle_object_genlist_add(glist, ad->csurf);

	eext_circle_object_genlist_scroller_policy_set(circle_glist,
						ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(circle_glist, EINA_TRUE);

	return circle_glist;
}

lists_s
glist_create(appdata_s *ad, const glist_entry_s *entries, size_t size) {
	Evas_Object *glist = elm_genlist_add(ad->navif);
	Evas_Object *circle_glist = circle_glist_create(ad, glist);

	for (size_t i = 0; i < size; ++i) {
		const glist_entry_s *entry = entries + i;
		glist_append(glist, entry->style, (void*)entry->text, entry->clicked_cb, (void*)ad);
	}

	evas_object_show(glist);

	lists_s retval = {glist, circle_glist};

	return retval;
}

