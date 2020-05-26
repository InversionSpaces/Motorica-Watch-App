/*
 * gllist.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef GLIST_H_
#define GLIST_H_

#include "motorica.h"

typedef struct {
	const char* text;
	const char* style;
	Evas_Smart_Cb clicked_cb;
} glist_entry_s;

typedef struct {
	Evas_Object *glist;
	Evas_Object *circle_glist;
} lists_s;

lists_s
glist_create(appdata_s *ad, const glist_entry_s *entries, size_t size);


#endif /* GLIST_H_ */
