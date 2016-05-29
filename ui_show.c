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

void static add_separator(Evas_Object *box) {
    Evas_Object *spacer, *separator;

    spacer = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_size_hint_min_set(spacer, 0, 15);
    evas_object_color_set(spacer, 0, 0, 0, 0);
    elm_box_pack_end(box, spacer);

    separator = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_color_set(separator, 60, 60, 60, 255);
    evas_object_size_hint_min_set(separator, 200, 1);
    evas_object_size_hint_weight_set(separator, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(separator, EVAS_HINT_FILL, 0);
    evas_object_show(separator);
    elm_box_pack_end(box, separator);
    
    spacer = evas_object_rectangle_add(evas_object_evas_get(box));
    evas_object_size_hint_min_set(spacer, 0, 15);
    evas_object_color_set(spacer, 0, 0, 0, 0);
    elm_box_pack_end(box, spacer);
}

static Evas_Object *create_contact_page(ui_show_page_data_s *page_data) {
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

    Evas_Object *delete_btn;

    vbox = elm_box_add(page_data->appdata->win);
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
    snprintf(id, id_len, "%d", page_data->contact->id);
    id_value_label = elm_label_add(id_hbox);
    elm_object_text_set(id_value_label, id);
    evas_object_size_hint_weight_set(id_value_label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(id_value_label, EVAS_HINT_FILL, 0);
    evas_object_show(id_value_label);
    elm_box_pack_end(id_hbox, id_value_label);

    elm_box_pack_end(vbox, id_hbox);
    
    add_separator(vbox);

    //name
    name_hbox = elm_box_add(vbox);
    elm_box_horizontal_set(name_hbox, EINA_TRUE);
    elm_box_padding_set(name_hbox, 5, 5);
    evas_object_size_hint_weight_set(name_hbox, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(name_hbox, EVAS_HINT_FILL, 0);
    evas_object_show(name_hbox);

    name_label = elm_label_add(name_hbox);
    elm_object_text_set(name_label, "Name: ");
    evas_object_size_hint_weight_set(name_label, 0, 0);
    evas_object_size_hint_align_set(name_label, 0, 0);
    evas_object_show(name_label);
    elm_box_pack_end(name_hbox, name_label);

    name_value_label = elm_label_add(name_hbox);
    elm_object_text_set(name_value_label, page_data->contact->display_name);
    evas_object_size_hint_weight_set(name_value_label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(name_value_label, EVAS_HINT_FILL, 0);
    evas_object_show(name_value_label);
    elm_box_pack_end(name_hbox, name_value_label);

    elm_box_pack_end(vbox, name_hbox);
    add_separator(vbox);

    //phone
    phone_hbox = elm_box_add(vbox);
    elm_box_horizontal_set(phone_hbox, EINA_TRUE);
    elm_box_padding_set(phone_hbox, 5, 5);
    evas_object_size_hint_weight_set(phone_hbox, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(phone_hbox, EVAS_HINT_FILL, 0);
    evas_object_show(phone_hbox);

    phone_label = elm_label_add(phone_hbox);
    elm_object_text_set(phone_label, "Phone: ");
    evas_object_size_hint_weight_set(phone_label, 0, 0);
    evas_object_size_hint_align_set(phone_label, 0, 0);
    evas_object_show(phone_label);
    elm_box_pack_end(phone_hbox, phone_label);

    phone_value_label = elm_label_add(phone_hbox);
    elm_object_text_set(phone_value_label, page_data->contact->phone);
    evas_object_size_hint_weight_set(phone_value_label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(phone_value_label, EVAS_HINT_FILL, 0);
    evas_object_show(phone_value_label);
    elm_box_pack_end(phone_hbox, phone_value_label);

    elm_box_pack_end(vbox, phone_hbox);
    add_separator(vbox);
    
    
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
