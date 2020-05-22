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
#define PACKAGE "org.example.motorica"
#endif

typedef struct appdata {
	/* Base GUI */
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *navif;
	Eext_Circle_Surface *csurf;
	/* -------- */

	int depth;
} appdata_s;

void
get_res_path(const char* path, char* out, int max_len);

#define SIZE(x) sizeof(x) / sizeof(0[x])

#endif /* __motorica_H__ */
