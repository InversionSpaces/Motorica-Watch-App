/*
 * gllist.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef GLIST_H_
#define GLIST_H_

#include <Elementary.h>

typedef struct {
	Evas_Object *list;
	Evas_Object *circle_list;
} glist_s;

char*
simple_text_get(void *data, Evas_Object *obj, const char *part);

Elm_Object_Item *
glist_append(glist_s glist, const char *style,
		     Elm_Gen_Item_Text_Get_Cb text_get_cb, const void *text_data,
			 Evas_Smart_Cb clicked_cb, const void *clicked_data);

#define glist_append_title(glist, title) \
	glist_append(glist, "title", simple_text_get, (const void*)title, NULL, NULL)

#define glist_append_entry(glist, text, clicked_cb, cb_data) \
	glist_append(glist, "1text", simple_text_get, (const void*)text, clicked_cb, cb_data)

#define glist_append_padding(glist) \
	glist_append(glist, "padding", NULL, NULL, NULL, NULL)

glist_s
glist_create(Evas_Object* parent, Eext_Circle_Surface* circle_surface);

#endif /* GLIST_H_ */
