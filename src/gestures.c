/*
 * gestures.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "gestures.h"

typedef struct {
	const char* img;
} sentry;

sentry sentries[] = {
		{"gestures/3-hippie.png"},
		{"gestures/4-fist.png"},
		{"gestures/9-pistol.png"}
};

void
clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	dlog_print(DLOG_INFO, LOG_TAG, "%s clicked", (const char*) data);
}

void
rselector_add_item(Evas_Object *rot_sel, const char* img) {
	Eext_Object_Item *it = eext_rotary_selector_item_append(rot_sel);

	char img_path[256] = {0,};
	get_res_path(img, img_path, sizeof(img_path));

	Evas_Object *image = elm_image_add(rot_sel);
	elm_image_file_set(image, img_path, NULL);

	eext_rotary_selector_item_part_content_set(it, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, image);

	image = elm_image_add(rot_sel);
	elm_image_file_set(image, img_path, NULL);

	eext_rotary_selector_item_part_content_set(it, "selector,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, image);

	evas_object_smart_callback_add(image, "clicked", clicked_cb, img);
}

void push_gestures(appdata_s *ad) {
	Evas_Object *rot_sel = eext_rotary_selector_add(ad->navif);
	eext_rotary_object_event_activated_set(rot_sel, EINA_TRUE);

	for (int i = 0; i < SIZE(sentries); ++i) {
		rselector_add_item(rot_sel, sentries[i].img);
	}

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, rot_sel, "empty");
	ad->depth++;
}



