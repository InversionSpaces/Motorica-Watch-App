/*
 * gllist.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef GLIST_H_
#define GLIST_H_

#include <Elementary.h>

/*
 * General list struct
 */
typedef struct {
	Evas_Object *list;
	Evas_Object *circle_list;
} glist_s;

/*
 * Create general list, if allocationg fails, returns NULL
 */
glist_s*
glist_create(Evas_Object *parent, Eext_Circle_Surface *circle_surface,
				Evas_Smart_Cb clicked_cb, void *cb_data);

/*
 * Append element to general list
 */
Elm_Object_Item *
glist_append(glist_s *glist, const char *style, Elm_Gen_Item_Text_Get_Cb text_get_cb, void *item_data);

/*
 * Insert element to general list after first element (after title)
 */
Elm_Object_Item *
glist_insert_after_first(glist_s *glist, const char *style, Elm_Gen_Item_Text_Get_Cb text_get_cb, void *item_data);

/*
 * Type of free function for general list item data
 */
typedef void (*free_func_t)(void*);

/*
 * Clear general list with given free function applied to each element data
 */
void
glist_clear(glist_s *glist, free_func_t free_func);

/*
 * Free general list with given free function applied to each element data
 */
void
glist_free(glist_s *glist, free_func_t free_func);

#endif /* GLIST_H_ */
