#ifndef __motorica_H__
#define __motorica_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#include "bt.h"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "motorica"

#if !defined(PACKAGE)
#define PACKAGE "org.motorica"
#endif

typedef struct appdata {
	/* Base GUI */
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *navif;
	Eext_Circle_Surface *csurf;
	/* -------- */

	/* Main menu */
	Evas_Object *glist;
	Evas_Object *circle_glist;

	/* Gestures */
	Evas_Object *gestures_box;
	Evas_Object *gestures_scroller;
	Evas_Object *gestures_circle_scroller;
	Evas_Object *gestures_padding_start;
	Evas_Object *gestures_padding_end;

	/* Devices */
	Evas_Object *devices_glist;
	Evas_Object *devices_circle_glist;

	enum {
		MAIN_MENU,
		DEVICES,
		GESTURES
	} state;
} appdata_s;

void
get_res_path(const char* path, char* out, int max_len);

#define SIZE(x) sizeof(x) / sizeof(0[x])

#endif /* __motorica_H__ */
