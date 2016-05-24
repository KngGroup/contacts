#include "ui_create.h"

Evas_Object *ui_create_page(Evas_Object *parent) {
    
}

void ui_show_create_page(appdata_s *ad) {
    Evas_Object *page = ui_create_page(ad->win);
    
    elm_naviframe_item_push(ad->naviframe, "Create", NULL, NULL, page, NULL);
}