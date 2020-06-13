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

glist_s*
glist_create(Evas_Object *parent, Eext_Circle_Surface *circle_surface,
				Evas_Smart_Cb clicked_cb, void *cb_data);

Elm_Object_Item *
glist_append(glist_s *glist, const char *style, Elm_Gen_Item_Text_Get_Cb text_get_cb, void *item_data);

Elm_Object_Item *
glist_insert_after_first(glist_s *glist, const char *style, Elm_Gen_Item_Text_Get_Cb text_get_cb, void *item_data);

typedef void (*free_func_t)(void*);

void
glist_clear(glist_s *glist, free_func_t free_func);

void
glist_free(glist_s *glist, free_func_t free_func);

#endif /* GLIST_H_ */
