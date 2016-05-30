#include "ui_show.h"

typedef struct ui_show_page_data {
    appdata_s *appdata;
    contact_s *contact;
} ui_show_page_data_s;

void delete_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    ui_show_page_data_s *page_data = (ui_show_page_data_s *) data;
    contact_repository_delete(page_data->contact->id);
    elm_naviframe_item_pop(page_data->appdata->naviframe);
    free(page_data);
}

void static box_append_separator(Evas_Object *box) {
    Evas_Object *spacer, *separator;
    
    spacer = elm_ext_spacer_add(box);
    elm_box_pack_end(box, spacer);
    
    separator = elm_ext_separator_add(box);
    elm_box_pack_end(box, separator);
    evas_object_show(separator);
    
    spacer = elm_ext_spacer_add(box);
    elm_box_pack_end(box, spacer);
}

Evas_Object static *box_append_label(Evas_Object *box, char *label_text) {
    Evas_Object *label;
    
    label = elm_label_add(box);
    elm_object_text_set(label, label_text);
    evas_object_color_set(label, 85, 135, 170, 255);
    evas_object_size_hint_weight_set(label, 0, 0);
    evas_object_size_hint_align_set(label, 0, 0);
    evas_object_show(label);
    elm_box_pack_end(box, label);
    
    return label;
}

Evas_Object static *box_append_label_value(Evas_Object *box, char *label_text) {
    Evas_Object *label;
    
    label = elm_label_add(box);
    elm_object_text_set(label, label_text);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, 0);
    evas_object_show(label);
    elm_box_pack_end(box, label);
    
    return label;
}

static Evas_Object *create_contact_page(ui_show_page_data_s *page_data) {
    Evas_Object *vbox;
    Evas_Object *spacer;
    Evas_Object *delete_btn;

    vbox = elm_box_add(page_data->appdata->win);
    elm_box_align_set(vbox, 0, 0);
    evas_object_show(vbox);
    
    spacer = elm_ext_spacer_add(vbox);
    elm_box_pack_end(vbox, spacer);
    
    box_append_label(vbox, "ID: ");
    size_t id_len = 10;
    char id[id_len];
    snprintf(id, id_len, "%d", page_data->contact->id);
    box_append_label_value(vbox, id);
    box_append_separator(vbox);

    box_append_label(vbox, "Name: ");
    box_append_label_value(vbox, page_data->contact->display_name);
    box_append_separator(vbox);

    box_append_label(vbox, "Phone: ");
    box_append_label_value(vbox, page_data->contact->phone);
    spacer = elm_ext_spacer_add(vbox);
    elm_box_pack_end(vbox, spacer);
    
    delete_btn = elm_button_add(vbox);
    elm_object_text_set(delete_btn, "Delete");
    evas_object_smart_callback_add(delete_btn, "clicked", delete_btn_on_click, page_data);
    evas_object_size_hint_weight_set(delete_btn, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(delete_btn, EVAS_HINT_FILL, 0);
    evas_object_show(delete_btn);
    elm_box_pack_end(vbox, delete_btn);

    return vbox;
}

void ui_show_contact_page(appdata_s *ad, contact_s *contact) {
    free(contact->phone);
    contact->phone = contact_repository_get_phone_by_id(contact->id);

    ui_show_page_data_s *page_data = malloc(sizeof (ui_show_page_data_s));
    page_data->appdata = ad;
    page_data->contact = contact;

    Evas_Object *contact_page = create_contact_page(page_data);

    elm_naviframe_item_push(ad->naviframe, "Show", NULL, NULL, contact_page, NULL);
}
