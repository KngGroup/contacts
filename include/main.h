#ifndef MAIN_H
#define MAIN_H

#include <dlog.h>
#include <Elementary.h>
#include <Eina.h>
#include <contacts.h>
#include "config.h"
#include "elm_ext.h"
#include "contact_repository.h"

typedef struct appdata {
    Evas_Object *win;
    Evas_Object *conformant;
    Evas_Object *naviframe;
} appdata_s;

#endif /* MAIN_H */