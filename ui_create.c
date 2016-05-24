#include "ui_create.h"

typedef struct contact_form {
    Evas_Object *firstname;
    Evas_Object *lastname;
    Evas_Object *phone;
    appdata_s *ad;
} contact_form_s;

void create_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    contact_form_s *contact_form = (contact_form_s *) data;
    contact_s contact;
    contact.firstname = strdup(elm_object_text_get(contact_form->firstname));
    contact.lastname = strdup(elm_object_text_get(contact_form->lastname));
    contact.phone = strdup(elm_object_text_get(contact_form->phone));
    
    contact_repository_create(&contact);
    elm_naviframe_item_pop(contact_form->ad->naviframe);
    
    free(contact_form);
}

Evas_Object *ui_create_page(appdata_s *ad) {
    contact_form_s *contact_form;
    Evas_Object *vbox;
    Evas_Object *create_btn;
    
    vbox = elm_box_add(ad->win);
    elm_box_align_set(vbox, 0, 0);
    evas_object_show(vbox);
    contact_form = malloc(sizeof(contact_form_s));
    
    contact_form->firstname = elm_entry_add(vbox);
    contact_form->lastname  = elm_entry_add(vbox);
    contact_form->phone     = elm_entry_add(vbox);
    contact_form->ad        = ad;
    create_btn              = elm_button_add(vbox);
    elm_object_text_set(create_btn, "Create");
    evas_object_smart_callback_add(create_btn, "clicked", create_btn_on_click, contact_form);
    
    elm_object_part_text_set(contact_form->firstname, "guide", "Firstname");
    elm_object_part_text_set(contact_form->lastname, "guide", "Lastname");
    elm_object_part_text_set(contact_form->phone, "guide", "Phone");
    
    elm_entry_single_line_set(contact_form->firstname, EINA_TRUE);
    elm_entry_single_line_set(contact_form->lastname, EINA_TRUE);
    elm_entry_single_line_set(contact_form->phone, EINA_TRUE);
    
    evas_object_size_hint_weight_set(contact_form->firstname, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_weight_set(contact_form->lastname, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_weight_set(contact_form->phone, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_weight_set(create_btn, EVAS_HINT_EXPAND, 0);
    
    evas_object_size_hint_align_set(contact_form->firstname, EVAS_HINT_FILL, 0);
    evas_object_size_hint_align_set(contact_form->lastname, EVAS_HINT_FILL, 0);
    evas_object_size_hint_align_set(contact_form->phone, EVAS_HINT_FILL, 0);
    evas_object_size_hint_align_set(create_btn, EVAS_HINT_FILL, 0);
    
    evas_object_show(contact_form->firstname);
    evas_object_show(contact_form->lastname);
    evas_object_show(contact_form->phone);
    evas_object_show(create_btn);
    
    elm_box_pack_end(vbox, contact_form->firstname);
    elm_box_pack_end(vbox, contact_form->lastname);
    elm_box_pack_end(vbox, contact_form->phone);
    elm_box_pack_end(vbox, create_btn);
    
    return vbox;
}

void ui_show_create_page(appdata_s *ad) {
    Evas_Object *page = ui_create_page(ad);
    
    elm_naviframe_item_push(ad->naviframe, "Create", NULL, NULL, page, NULL);
}