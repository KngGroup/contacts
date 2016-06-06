#include "ui_create.h"

typedef struct contact_form {
    Evas_Object *firstname;
    Evas_Object *lastname;
    Eina_List *numbers;
    Eina_List *emails;
    Evas_Object *note;
    Evas_Object *vbox;
    appdata_s *ad;
    ui_list_page_data_s *ui_list_page_data;
} contact_form_s;

static char *phone_type_labels[] = {"Main", "Work", "Assistant", "Callback", "Car", "Pager", "Other", NULL};
static contacts_number_type_e phone_types[] = {CONTACTS_NUMBER_TYPE_MAIN, CONTACTS_NUMBER_TYPE_COMPANY_MAIN, CONTACTS_NUMBER_TYPE_ASSISTANT, CONTACTS_NUMBER_TYPE_ISDN, CONTACTS_NUMBER_TYPE_CAR, CONTACTS_NUMBER_TYPE_PAGER, CONTACTS_NUMBER_TYPE_OTHER};

static const char *get_phone_type_label(contacts_number_type_e phone_type) {
    int i;
    for (i = 0; phone_types[i] != CONTACTS_NUMBER_TYPE_OTHER && phone_types[i] != phone_type; i++) {
    }
    
    return phone_type_labels[i];
}
static contacts_number_type_e get_phone_type_by_label(const char *phone_type_label) {
    int i;
    for (i = 0; phone_type_labels[i] != NULL && strcmp(phone_type_labels[i], phone_type_label); i++) {
    }
    
    return phone_types[i];
}

Evas_Object *elm_box_get_child_by_name(Evas_Object *box, const char *name) {
    Eina_List *children;
    Eina_List *l;
    Evas_Object *child;
    
    children = elm_box_children_get(box);
    
    EINA_LIST_FOREACH(children, l, child) {
        if (!strcmp(name, evas_object_name_get(child))) {
            return child;
        }
    }
    
    return NULL;
}

void create_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    Eina_List *l;
    Evas_Object *phone_hbox;
    Evas_Object *email_entry;
    contact_s *contact = contact_repository_contact_s_new();
    
    contact_form_s *contact_form = (contact_form_s *) data;
    
    contact->firstname = strdup(elm_object_text_get(contact_form->firstname));
    contact->lastname = strdup(elm_object_text_get(contact_form->lastname));
    contact->note = strdup(elm_object_text_get(contact_form->note));
    
    EINA_LIST_FOREACH(contact_form->numbers, l, phone_hbox) {
            phone_s *phone = contact_repository_phone_s_new();
            phone->number = strdup(elm_object_text_get(elm_box_get_child_by_name(phone_hbox, "number_entry")));
            phone->type = get_phone_type_by_label(elm_object_text_get(elm_box_get_child_by_name(phone_hbox, "type_label")));
            contact->phone_numbers = eina_list_append(contact->phone_numbers, phone);
    }
    
    EINA_LIST_FOREACH(contact_form->emails, l, email_entry) {
        contact->emails = eina_list_append(contact->emails, strdup(elm_object_text_get(email_entry)));
    }
    
    contact_repository_create(contact);
    contact_repository_contact_s_free(contact);
    
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

static void phone_on_change(void *data, Evas_Object *obj, void *event_info);

static Evas_Object *ui_append_radio(Evas_Object *vbox, int value, char *text) {
    Evas_Object *hbox;
    Evas_Object *radio;
    Evas_Object *label;
    
    hbox = elm_box_add(vbox);
    evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_horizontal_set(hbox, EINA_TRUE);
    
    radio = elm_radio_add(hbox);
    evas_object_size_hint_align_set(radio, 0, 0);
    elm_radio_state_value_set(radio, value);
    elm_box_pack_end(hbox, radio);
    
    label = elm_label_add(hbox);
    elm_object_text_set(label, text);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, 0.5);
    elm_box_pack_end(hbox, label);
    evas_object_show(radio);
    evas_object_show(label);
    evas_object_show(hbox);
    elm_box_pack_end(vbox, hbox);
    
    return radio;
}

static void on_phone_type_changed(void *data, Evas_Object *obj, void *event_info) {
    Evas_Object *phone_type_label = data;
    const char *label = get_phone_type_label(elm_radio_value_get(obj));
    elm_object_text_set(phone_type_label, label);
    
    elm_popup_timeout_set(evas_object_data_get(phone_type_label, "popup"), 0.1);
}

static void on_phone_type_label_clicked(void *data, Evas_Object *obj, void *event_info) {
    int i;
    Evas_Object *popup;
    Evas_Object *vbox;
    Evas_Object *radio_group;
    Evas_Object *radio;
    contact_form_s *contact_form = data;
    
    
    popup = elm_popup_add(contact_form->ad->naviframe);
    elm_object_part_text_set(popup, "title,text", "Phone type");
    evas_object_data_set(obj, "popup", popup);

    vbox = elm_box_add(popup);
    
    radio_group = ui_append_radio(vbox, phone_types[0], phone_type_labels[0]);
    evas_object_smart_callback_add(radio_group, "changed", on_phone_type_changed, obj);
    
    for (i = 1; phone_type_labels[i] != NULL; i++) {
        radio = ui_append_radio(vbox, phone_types[i], phone_type_labels[i]);
        elm_radio_group_add(radio, radio_group);
        evas_object_smart_callback_add(radio, "changed", on_phone_type_changed, obj);
    }
    
    elm_radio_value_set(radio_group, get_phone_type_by_label(elm_object_text_get(obj)));
    evas_object_show(vbox);
    
    elm_object_content_set(popup, vbox);
    evas_object_show(popup);
}

static void *ui_append_phone_entry_after(contact_form_s *contact_form, Evas_Object *after) {
    Evas_Object *hbox;
    Evas_Object *number_entry;
    Evas_Object *type_label;
    
    hbox = elm_box_add(contact_form->vbox);
    evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_horizontal_set(hbox, EINA_TRUE);
    
    number_entry = ui_create_entry(hbox, "Enter phone");
    evas_object_smart_callback_add(number_entry, "changed,user", phone_on_change, contact_form);
    elm_entry_input_panel_layout_set(number_entry, ELM_INPUT_PANEL_LAYOUT_PHONENUMBER);
    evas_object_show(number_entry);
    elm_box_pack_end(hbox, number_entry);
    
    
    type_label = elm_label_add(hbox);
    elm_object_text_set(type_label, phone_type_labels[0]);
    evas_object_size_hint_align_set(type_label, 0, 0.5);
    evas_object_size_hint_weight_set(type_label, 0.2, 0);
    evas_object_show(type_label);
    elm_box_pack_start(hbox, type_label);
    evas_object_smart_callback_add(type_label, "clicked", on_phone_type_label_clicked, contact_form);
    
    evas_object_name_set(number_entry, "number_entry");
    evas_object_name_set(type_label, "type_label");
    
    contact_form->numbers = eina_list_append(contact_form->numbers, hbox);
    evas_object_show(hbox);
    elm_box_pack_after(contact_form->vbox, hbox, after);
}

static void phone_on_change(void *data, Evas_Object *obj, void *event_info) {
    evas_object_smart_callback_del(obj, "changed,user", phone_on_change);
    
    contact_form_s *contact_form = (contact_form_s *) data;
    Eina_List *l = eina_list_last(contact_form->numbers);
    Evas_Object *last_phone_entry = (Evas_Object *)eina_list_data_get(l);
    ui_append_phone_entry_after(contact_form, last_phone_entry);
} 

static void email_on_change(void *data, Evas_Object *obj, void *event_info) {
    contact_form_s *contact_form = (contact_form_s *) data;
    Evas_Object *entry = ui_create_entry(contact_form->vbox, "Email");
    elm_entry_input_panel_layout_set(entry, ELM_INPUT_PANEL_LAYOUT_EMAIL);
    
    Eina_List *l = eina_list_last(contact_form->emails);
    Evas_Object *last_entry = (Evas_Object *)eina_list_data_get(l);
    contact_form->emails = eina_list_append(contact_form->emails, entry);
    evas_object_smart_callback_add(entry, "changed,user", email_on_change, contact_form);
    evas_object_smart_callback_del(obj, "changed,user", email_on_change);
    
    elm_box_pack_after(contact_form->vbox, entry, last_entry);
}

static Evas_Object *ui_create_page(appdata_s *ad, ui_list_page_data_s *ui_list_page_data) {
    contact_form_s *contact_form;
    Evas_Object *vbox;
    Evas_Object *scroller;
    Evas_Object *create_btn;
    Evas_Object *entry;
    Evas_Object *separator;
    
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
    separator = elm_ext_vbox_add_separator(vbox);
    
    ui_append_phone_entry_after(contact_form, separator);
    elm_ext_vbox_add_separator(vbox);
    
    entry = ui_create_entry(vbox, "Email");
    contact_form->emails = eina_list_append(contact_form->emails, entry);
    evas_object_smart_callback_add(entry, "changed,user", email_on_change, contact_form);
    elm_entry_input_panel_layout_set(entry, ELM_INPUT_PANEL_LAYOUT_EMAIL);
    elm_box_pack_end(vbox, entry);
    elm_ext_vbox_add_separator(vbox);
    
    contact_form->note = ui_create_entry(vbox, "Note");
    elm_entry_single_line_set(contact_form->note, EINA_FALSE);
    elm_box_pack_end(vbox, contact_form->note);
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