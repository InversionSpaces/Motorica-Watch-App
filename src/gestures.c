/*
 * gestures.c
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#include "gestures.h"

/*
 * GUI Elements
 */
static Evas_Object *box = NULL;
static Evas_Object *scroller = NULL;
static Evas_Object *circle_scroller = NULL;
static Evas_Object *padding_start = NULL;
static Evas_Object *padding_end = NULL;

/*
 * Scroller entry struct
 */
typedef struct {
	const char* img;
} sentry;

/*
 * Scroller entries
 */
static sentry sentries[] = {
		{"gestures/3-hippie.png"},
		{"gestures/4-fist.png"},
		{"gestures/9-pistol.png"}
};

/*
 * Create image object from path to image
 */
static Evas_Object *
image_create(Evas_Object *parent, const char *path);

/*
 * Create page layout with image from path to image
 */
static Evas_Object *
page_layout_create(Evas_Object *parent, const char *img);

/*
 * Create padding object
 */
static Evas_Object *
padding_item_create(Evas_Object *parent);

/*
 * Callback for layout "resize" event
 */
static void
layout_resize_cb(void *data , Evas *e, Evas_Object *page, void *event_info);

/*-*/

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
	// Creating layout
	Evas_Object *page_layout = elm_layout_add(parent);

	elm_layout_theme_set(page_layout, "layout", "body_thumbnail", "default");

	evas_object_size_hint_weight_set(page_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(page_layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

	evas_object_show(page_layout);

	// Creating image
	Evas_Object *image = image_create(page_layout, img);

	// Filling layout part with image
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

   evas_object_geometry_get(page, NULL, NULL, &page_width, &page_height);
   evas_object_geometry_get(scroller, NULL, NULL, &container_width, &container_height);

   int padding_size = (container_width - page_width) / 2;

   evas_object_size_hint_min_set(padding_start, padding_size, container_height);
   evas_object_size_hint_min_set(padding_end, padding_size, container_height);

   elm_scroller_page_size_set(scroller, page_width, page_height);
}

static void
box_create(Evas_Object *parent)
{
	box = elm_box_add(parent);

	Evas_Object *page;
	for (int i = 0; i < SIZE(sentries); ++i) {
		page = page_layout_create(box, sentries[i].img);
		elm_box_pack_end(box, page);
	}

	padding_start = padding_item_create(box);
	elm_box_pack_start(box, padding_start);

	padding_end = padding_item_create(box);
	elm_box_pack_end(box, padding_end);

	evas_object_event_callback_add(page, EVAS_CALLBACK_RESIZE, layout_resize_cb, NULL);

	elm_box_horizontal_set(box, EINA_TRUE);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(box);
}

static void
circle_scroller_create(Evas_Object *scroller, Eext_Circle_Surface *circle_surface) {
	circle_scroller = eext_circle_object_scroller_add(scroller, circle_surface);

	eext_circle_object_genlist_scroller_policy_set(
		circle_scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO
	);
	eext_rotary_object_event_activated_set(circle_scroller, EINA_TRUE);
}

static void
scroller_create(Evas_Object *parent, Eext_Circle_Surface *circle_surface) {
	scroller = elm_scroller_add(parent);

	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
	evas_object_show(scroller);

	box_create(scroller);
	elm_object_content_set(scroller, box);

	circle_scroller_create(scroller, circle_surface);
}

void push_gestures(appdata_s *ad) {
	// Creating scroller
	scroller_create(ad->navif, ad->csurf);

	// Pushing it to naviframe stack
	elm_naviframe_item_push(ad->navif, NULL, NULL, NULL, scroller, "empty");

	// Updating app state
	ad->state = GESTURES;
}


