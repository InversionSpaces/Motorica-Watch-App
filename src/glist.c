/*
 * glist.c
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#include <efl_extension.h>
#include <dlog.h>

#include "glist.h"
#include "motorica.h"

char*
simple_text_get(void *data, Evas_Object *obj, const char *part) {
	const char* text = data;

	if (text == NULL) return NULL;
	return strdup(text);
}

Elm_Object_Item *
glist_append(glist_s glist, const char *style,
		     Elm_Gen_Item_Text_Get_Cb text_get_cb, void *text_data,
			 Evas_Smart_Cb clicked_cb, void *clicked_data) {
	if (glist.list == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "glist.list is NULL");
		return NULL;
	}

	if (style == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "style is NULL");
		return NULL;
	}

	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

	itc->item_style = style;
	itc->func.text_get = text_get_cb;

	Elm_Object_Item* item = elm_genlist_item_append(
				glist.list, itc, text_data,
				NULL, ELM_GENLIST_ITEM_NONE,
				clicked_cb, clicked_data
			);

	elm_genlist_item_class_free(itc);

	return item;
}

Elm_Object_Item *
glist_insert_after_first(glist_s glist, const char *style,
		     Elm_Gen_Item_Text_Get_Cb text_get_cb, void *text_data,
			 Evas_Smart_Cb clicked_cb, void *clicked_data) {
	if (glist.list == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "glist.list is NULL");
		return NULL;
	}

	if (style == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "style is NULL");
		return NULL;
	}

	Elm_Widget_Item* first = elm_genlist_first_item_get(glist.list);

	if (first == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "elm_genlist_first_item_get is NULL");
		return NULL;
	}

	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

	itc->item_style = style;
	itc->func.text_get = text_get_cb;

	Elm_Object_Item* item = elm_genlist_item_insert_after(
				glist.list, itc, text_data,
				NULL, first, ELM_GENLIST_ITEM_NONE,
				clicked_cb, clicked_data
			);

	elm_genlist_item_class_free(itc);

	return item;
}

static Evas_Object *
circle_glist_create(Evas_Object *list, Eext_Circle_Surface* circle_surface) {
	Evas_Object *circle_list = eext_circle_object_genlist_add(list, circle_surface);

	eext_circle_object_genlist_scroller_policy_set(circle_list, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(circle_list, EINA_TRUE);

	return circle_list;
}

glist_s
glist_create(Evas_Object* parent, Eext_Circle_Surface* circle_surface) {
	Evas_Object *list = elm_genlist_add(parent);
	Evas_Object *circle_list = circle_glist_create(list, circle_surface);

	evas_object_show(list);

	glist_s retval = {list, circle_list};

	return retval;
}

void
glist_clear(glist_s glist) {
	elm_genlist_clear(glist.list);
}

