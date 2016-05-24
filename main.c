#include <stdio.h>
#include <dlog.h>
#include <Elementary.h>
#include <contacts.h>
#include <math.h>
#include "config.h"

typedef struct appdata {
    Evas_Object *win;
    Evas_Object *naviframe;
} appdata_s;

create_contacts() {
    contacts_record_h contact;
    contacts_record_h name;
    contacts_record_h number;
    int i;
    contacts_connect_on_thread();

    for (i = 0; i < 10; i++) {
        int id = -1;

        contacts_record_create(_contacts_contact._uri, &contact);
        contacts_record_create(_contacts_name._uri, &name);

        contacts_record_set_str(name, _contacts_name.first, "John");
        contacts_record_set_str(name, _contacts_name.last, "Smith");
        contacts_record_add_child_record(contact, _contacts_contact.name, name);

        contacts_record_create(_contacts_number._uri, &number);
        contacts_record_set_str(number, _contacts_number.number, "+71111111111");
        contacts_record_add_child_record(contact, _contacts_contact.number, number);

        contacts_db_insert_record(contact, &id);
    }
}

static char *genlist_contacts_text_get(void *data, Evas_Object *obj, const char *part) {
    if (strcmp(part, "elm.text") == 0) {
        char *name = (char *) data;

        return strdup(name);
    } else {
        return NULL;
    }
}

static Evas_Object *genlist_contacts_content_get(void *data, Evas_Object *obj, const char *part) {
    if (!strcmp(part, "elm.swallow.icon")) {
        // Create a new icon
        Evas_Object *ic = elm_icon_add(obj);
        // Set the filename of the file which is to be loaded
        elm_image_file_set(ic, "/home/developer/acc.png", NULL);
        // Keep the ratio squared
        evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

        // Return the icon
        return ic;

    }

    return NULL;
}

static void create_contact_show_page(appdata_s *appdata, char *name) {
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

    dlog_print(DLOG_DEBUG, PACKAGE, "Show contact: %s", name);

    elm_naviframe_item_push(appdata->naviframe, "Edit Contact", NULL, NULL, vbox, NULL);
}

static void genlist_contacts_selected_cb(void *data, Evas_Object *obj, void *event_info) {
    appdata_s *ad = data;

    Elm_Object_Item *it = (Elm_Object_Item*) event_info;

    char *name = strdup(elm_object_item_data_get(it));
    dlog_print(DLOG_DEBUG, PACKAGE, "Selected: %s", elm_object_item_data_get(it));
    create_contact_show_page(ad, name);
}

static Evas_Object *ui_create_contact_list(appdata_s *appdata) {
    Elm_Genlist_Item_Class *itc;
    Evas_Object *ui_list;
    char *display_name;
    contacts_record_h record;
    contacts_list_h c_list;

    itc = elm_genlist_item_class_new();
    itc->item_style = "default";
    itc->func.text_get = genlist_contacts_text_get;
    itc->func.content_get = genlist_contacts_content_get;
    itc->func.state_get = NULL;
    itc->func.del = NULL;

    ui_list = elm_genlist_add(appdata->win);

    contacts_connect_on_thread();
    contacts_db_get_all_records(_contacts_person._uri, 0, 0, &c_list);
    while (contacts_list_get_current_record_p(c_list, &record) == CONTACTS_ERROR_NONE) {
        contacts_record_get_str_p(record, _contacts_person.display_name, &display_name);
        elm_genlist_item_append(
                ui_list,
                itc,
                display_name,
                NULL,
                ELM_GENLIST_ITEM_NONE,
                genlist_contacts_selected_cb,
                appdata
                );

        contacts_list_next(c_list);
    }

    return ui_list;
}

void create_gui(appdata_s *ad) {
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);

    ad->naviframe = elm_naviframe_add(ad->win);
    elm_naviframe_prev_btn_auto_pushed_set(ad->naviframe, EINA_TRUE);
    evas_object_size_hint_weight_set(ad->naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->naviframe);
    evas_object_show(ad->naviframe);

    
    
    Evas_Object *btn = elm_button_add(ad->naviframe);
    Evas_Object *ic;
    ic = elm_icon_add(btn);
    elm_image_file_set(ic, "/home/developer/acc.png", NULL);
    elm_object_part_content_set(btn, "icon", ic);
    evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.5);
    
    Evas_Object *contact_list = ui_create_contact_list(ad);
    
    
    elm_naviframe_item_push(ad->naviframe, "Contacts", NULL, btn, contact_list, NULL);

    evas_object_show(ad->win);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
    appdata_s ad = {0,};
    create_gui(&ad);
    elm_run();
    return 0;
}
ELM_MAIN()
