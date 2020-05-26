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

static sentry sentries[] = {
		{"gestures/3-hippie.png"},
		{"gestures/4-fist.png"},
		{"gestures/9-pistol.png"}
};

static Evas_Object *
image_create(Evas_Object *parent, const char *path) {
	char img_path[256] = {0,};
	get_res_path(path, img_path, sizeof(img_path));

	Evas_Object *image = elm_image_add(parent);
	elm_image_file_set(image, img_path, NULL);

	evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(image, EVAS_HINT_FILL, EVAS_HINT_FILL);

	evas_object_show(image);

	return image;
}

static Evas_Object *
page_layout_create(Evas_Object *parent, const char *img)
{
   Evas_Object *page_layout = elm_layout_add(parent);
   elm_layout_theme_set(page_layout, "layout", "body_thumbnail", "default");
   evas_object_size_hint_weight_set(page_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(page_layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(page_layout);

   Evas_Object *image = image_create(page_layout, img);
   elm_object_part_content_set(page_layout, "elm.icon", image);

   return page_layout;
}

static Evas_Object *
padding_item_create(Evas_Object *parent)
{
   Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(parent));

   evas_object_size_hint_weight_set(padding, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(padding, EVAS_HINT_FILL, EVAS_HINT_FILL);

   return padding;
}

static void
layout_resize_cb(void *data , Evas *e, Evas_Object *page, void *event_info)
{
   int page_width;
   int page_height;
   int container_width;
   int container_height;
   appdata_s *ad = data;

   evas_object_geometry_get(page, NULL, NULL, &page_width, &page_height);
   evas_object_geometry_get(ad->gestures_scroller, NULL, NULL, &container_width, &container_height);

   int padding_size = (container_width - page_width) / 2;
   evas_object_size_hint_min_set(ad->gestures_padding_start, padding_size, container_height);
   evas_object_size_hint_min_set(ad->gestures_padding_end, padding_size, container_height);

   elm_scroller_page_size_set(ad->gestures_scroller, page_width, page_height);
}

static Evas_Object *
box_create(appdata_s *ad)
{
	ad->gestures_box = elm_box_add(ad->gestures_scroller);

	Evas_Object *page;
	for (int i = 0; i < SIZE(sentries); ++i) {
		page = page_layout_create(ad->gestures_box, sentries[i].img);
		elm_box_pack_end(ad->gestures_box, page);
	}

	ad->gestures_padding_start = padding_item_create(ad->gestures_box);
	elm_box_pack_start(ad->gestures_box, ad->gestures_padding_start);
	ad->gestures_padding_end = padding_item_create(ad->gestures_box);
	elm_box_pack_end(ad->gestures_box, ad->gestures_padding_end);

	evas_object_event_callback_add(page, EVAS_CALLBACK_RESIZE, layout_resize_cb, ad);

	elm_box_horizontal_set(ad->gestures_box, EINA_TRUE);
	evas_object_size_hint_weight_set(ad->gestures_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(ad->gestures_box);

	return ad->gestures_box;
}

static Evas_Object *
circle_scroller_create(appdata_s *ad) {
	ad->gestures_circle_scroller = eext_circle_object_scroller_add(ad->gestures_scroller, ad->csurf);

	eext_circle_object_genlist_scroller_policy_set(ad->gestures_circle_scroller,
							ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(ad->gestures_circle_scroller, EINA_TRUE);

	return ad->gestures_circle_scroller;
}

static Evas_Object *
scroller_create(appdata_s *ad) {
	ad->gestures_scroller = elm_scroller_add(ad->navif);

	ad->gestures_box = box_create(ad);
	ad->gestures_circle_scroller = circle_scroller_create(ad);

	elm_object_content_set(ad->gestures_scroller, ad->gestures_box);
	elm_scroller_policy_set(ad->gestures_scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

	evas_object_show(ad->gestures_scroller);

	return ad->gestures_scroller;
}

void push_gestures(appdata_s *ad) {
	ad->gestures_scroller = scroller_create(ad);

	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, ad->gestures_scroller, "empty");

	ad->state = GESTURES;
}


