#include "ui_create.h"

typedef struct contact_form {
    Evas_Object *firstname;
    Evas_Object *lastname;
    Eina_List *numbers;
    Evas_Object *vbox;
    appdata_s *ad;
    ui_list_page_data_s *ui_list_page_data;
} contact_form_s;

void create_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    contact_form_s *contact_form = (contact_form_s *) data;
    contact_s contact;
    contact.firstname = strdup(elm_object_text_get(contact_form->firstname));
    contact.lastname = strdup(elm_object_text_get(contact_form->lastname));
    contact.phone = strdup(elm_object_text_get(eina_list_data_get(contact_form->numbers)));
    
    contact_repository_create(&contact);
    free(contact.firstname);
    free(contact.lastname);
    free(contact.phone);
    
    ui_list_refresh_contacts(contact_form->ui_list_page_data);
    elm_naviframe_item_pop(contact_form->ad->naviframe);
    
    free(contact_form);
}

static Evas_Object *ui_create_entry(Evas_Object *parent, const char* guide) {
    Evas_Object *entry = elm_entry_add(parent);
    elm_object_part_text_set(entry, "guide", guide);
    elm_entry_single_line_set(entry, EINA_TRUE);
    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, 0);
    evas_object_show(entry);
    
    return entry;
}

static void phone_on_change(void *data, Evas_Object *obj, void *event_info) {
    contact_form_s *contact_form = (contact_form_s *) data;
    Evas_Object *entry = ui_create_entry(contact_form->vbox, "Phone");
    elm_entry_input_panel_layout_set(entry, ELM_INPUT_PANEL_LAYOUT_PHONENUMBER);
    
    Eina_List *l = eina_list_last(contact_form->numbers);
    Evas_Object *last_entry = (Evas_Object *)eina_list_data_get(l);
    contact_form->numbers = eina_list_append(contact_form->numbers, entry);
    evas_object_smart_callback_add(entry, "changed,user", phone_on_change, contact_form);
    evas_object_smart_callback_del(obj, "changed,user", phone_on_change);
    
    elm_box_pack_after(contact_form->vbox, entry, last_entry);
} 

static Evas_Object *ui_create_page(appdata_s *ad, ui_list_page_data_s *ui_list_page_data) {
    contact_form_s *contact_form;
    Evas_Object *vbox;
    Evas_Object *scroller;
    Evas_Object *create_btn;
    Evas_Object *entry;
    
    vbox = elm_box_add(ad->win);
    elm_box_align_set(vbox, 0, 0);
    evas_object_show(vbox);
    contact_form = malloc(sizeof(contact_form_s));
    
    contact_form->ad                = ad;
    contact_form->ui_list_page_data = ui_list_page_data;
    contact_form->numbers           = NULL;
    contact_form->vbox              = vbox;
    
    elm_ext_vbox_add_separator(vbox);
    contact_form->firstname = ui_create_entry(vbox, "Firstname");
    elm_box_pack_end(vbox, contact_form->firstname);
    elm_ext_vbox_add_separator(vbox);
    
    contact_form->lastname = ui_create_entry(vbox, "Lastname");
    elm_box_pack_end(vbox, contact_form->lastname);
    elm_ext_vbox_add_separator(vbox);
    
    entry = ui_create_entry(vbox, "Phone");
    contact_form->numbers = eina_list_append(contact_form->numbers, entry);
    evas_object_smart_callback_add(entry, "changed,user", phone_on_change, contact_form);
    elm_entry_input_panel_layout_set(entry, ELM_INPUT_PANEL_LAYOUT_PHONENUMBER);
    elm_box_pack_end(vbox, entry);
    elm_ext_vbox_add_separator(vbox);
    
    create_btn = elm_button_add(vbox);
    evas_object_size_hint_weight_set(create_btn, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(create_btn, EVAS_HINT_FILL, 0);
    elm_object_text_set(create_btn, "Create");
    evas_object_show(create_btn);
    elm_box_pack_end(vbox, create_btn);
    evas_object_smart_callback_add(create_btn, "clicked", create_btn_on_click, contact_form);
    
    scroller = elm_scroller_add(ad->win);
    evas_object_show(scroller);
    elm_object_content_set(scroller, vbox);
    
    evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    
    return scroller;
}

void ui_show_create_page(appdata_s *ad, ui_list_page_data_s *ui_list_page_data) {
    Evas_Object *page = ui_create_page(ad, ui_list_page_data);
    
    elm_naviframe_item_push(ad->naviframe, "Create", NULL, NULL, page, NULL);
}