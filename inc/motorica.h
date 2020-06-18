#ifndef __motorica_H__
#define __motorica_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

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

	/* Device */
	struct {
		bool is_present;
		bool is_connected;

		char *name;
		char *address;
	};

	/* Gestures */
	Evas_Object *gestures_box;
	Evas_Object *gestures_scroller;
	Evas_Object *gestures_circle_scroller;
	Evas_Object *gestures_padding_start;
	Evas_Object *gestures_padding_end;

	/* App state */
	enum {
		MAIN_MENU,
		DEVICES,
		CONNECT_DEVICES,
		BOND_DEVICES,
		GESTURES
	} state;
} appdata_s;

void
get_res_path(const char* path, char* out, int max_len);

#define SIZE(x) sizeof(x) / sizeof(0[x])

#endif /* __motorica_H__ */
