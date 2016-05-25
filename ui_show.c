#include "ui_show.h"

static Evas_Object *create_contact_page(appdata_s *appdata, contact_s *contact) {
    Evas_Object *vbox;
    
    Evas_Object *id_hbox;
    Evas_Object *name_hbox;
    Evas_Object *phone_hbox;
    
    Evas_Object *id_label;
    Evas_Object *id_value_label;
    
    Evas_Object *name_label;
    Evas_Object *name_value_label;
    
    Evas_Object *phone_label;
    Evas_Object *phone_value_label;
    
    vbox = elm_box_add(appdata->win);
    elm_box_align_set(vbox, 0, 0);
    evas_object_show(vbox);

    //id
    id_hbox = elm_box_add(vbox);
    elm_box_horizontal_set(id_hbox, EINA_TRUE);
    elm_box_padding_set(id_hbox, 5, 5);
    evas_object_size_hint_weight_set(id_hbox, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(id_hbox, EVAS_HINT_FILL, 0);
    evas_object_show(id_hbox);
    
    id_label = elm_label_add(id_hbox);
    elm_object_text_set(id_label, "ID: ");
    evas_object_size_hint_weight_set(id_label, 0, 0);
    evas_object_size_hint_align_set(id_label, 0, 0);
    evas_object_show(id_label);
    elm_box_pack_end(id_hbox, id_label);
    
    size_t id_len = 10;
    char id[id_len];
    snprintf(id, id_len, "%d", contact->id);
    id_value_label = elm_label_add(id_hbox);
    elm_object_text_set(id_value_label, id);
    evas_object_size_hint_weight_set(id_value_label, 0, 0);
    evas_object_size_hint_align_set(id_value_label, 0, 0);
    evas_object_show(id_value_label);
    elm_box_pack_end(id_hbox, id_value_label);
    
    elm_box_pack_end(vbox, id_hbox);

    //name
    name_hbox = elm_box_add(vbox);
    elm_box_horizontal_set(name_hbox, EINA_TRUE);
    elm_box_padding_set(name_hbox, 5, 5);
    evas_object_size_hint_weight_set(name_hbox, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(name_hbox, EVAS_HINT_FILL, 0);
    evas_object_show(name_hbox);
    
    name_label = elm_label_add(name_hbox);
    elm_object_text_set(id_label, "Name: ");
    evas_object_size_hint_weight_set(name_label, 0, 0);
    evas_object_size_hint_align_set(name_label, 0, 0);
    evas_object_show(name_label);
    elm_box_pack_end(name_hbox, name_label);
    
    name_value_label = elm_label_add(name_hbox);
    elm_object_text_set(name_value_label, contact->display_name);
    evas_object_size_hint_weight_set(name_value_label, 0, 0);
    evas_object_size_hint_align_set(name_value_label, 0, 0);
    evas_object_show(name_value_label);
    elm_box_pack_end(name_hbox, name_value_label);
    
    elm_box_pack_end(vbox, name_hbox);

    return vbox;
}

static void back_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    appdata_s *ad = (appdata_s *) data;
    elm_naviframe_item_pop(ad->naviframe);
}

void ui_show_contact_page(appdata_s *ad, contact_s *contact) {
    Evas_Object *contact_page = create_contact_page(ad, contact);
    
    Evas_Object *back_btn = elm_button_add(ad->naviframe);
    elm_object_text_set(back_btn, "x");
    evas_object_size_hint_min_set(back_btn, 64, 64);
    evas_object_smart_callback_add(back_btn, "clicked", back_btn_on_click, ad);
    
    elm_naviframe_item_push(ad->naviframe, "Show", NULL, back_btn, contact_page, NULL);
}