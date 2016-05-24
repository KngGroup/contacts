#include "ui_show.h"

static Evas_Object *create_contact_page(appdata_s *appdata, char *name) {
    Evas_Object *vbox;
    Evas_Object *hbox;
    Evas_Object *label;

    vbox = elm_box_add(appdata->win);
    elm_box_align_set(vbox, 0, 0);
    evas_object_show(vbox);

    hbox = elm_box_add(vbox);
    elm_box_horizontal_set(hbox, EINA_TRUE);
    elm_box_padding_set(hbox, 32, 32);
    evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0);
    evas_object_show(hbox);

    label = elm_label_add(hbox);
    elm_object_text_set(label, name);
    evas_object_size_hint_weight_set(label, 0, 0);
    evas_object_size_hint_align_set(label, 0, 0);
    evas_object_show(label);

    elm_box_pack_end(hbox, label);
    elm_box_pack_end(vbox, hbox);

    return vbox;
}

void ui_show_contact_page(appdata_s *ad, char *name) {
    Evas_Object *contact_page = create_contact_page(ad, name);
    elm_naviframe_item_push(ad->naviframe, "Edit Contact", NULL, NULL, contact_page, NULL);
}