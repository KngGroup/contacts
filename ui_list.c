#include "ui_list.h"
#include "ui_show.h"
#include "ui_create.h"

static char *genlist_text_get(void *data, Evas_Object *obj, const char *part) {
    if (strcmp(part, "elm.text") == 0) {
        contact_s *contact = (contact_s *) data;

        return strdup(contact->display_name);
    } else {
        return NULL;
    }
}

static void genlist_selected_cb(void *data, Evas_Object *obj, void *event_info) {
    appdata_s *ad       = data;
    Elm_Object_Item *it = (Elm_Object_Item*) event_info;
    contact_s *contact  = elm_object_item_data_get(it);
    
    ui_show_contact_page(ad, contact);
}

static Evas_Object *ui_create_list(appdata_s *ad, Eina_List *contacts) {
    contact_s *contact;
    
    Elm_Genlist_Item_Class *itc;
    Evas_Object *ui_list;
    
    Eina_List *l;

    itc = elm_genlist_item_class_new();
    itc->item_style       = "default";
    itc->func.text_get    = genlist_text_get;
    itc->func.content_get = NULL;
    itc->func.state_get   = NULL;
    itc->func.del         = NULL;

    ui_list = elm_genlist_add(ad->win);
    EINA_LIST_FOREACH(contacts, l, contact) {
            elm_genlist_item_append(ui_list,
                                    itc,
                                    contact,
                                    NULL,
                                    ELM_GENLIST_ITEM_NONE,
                                    genlist_selected_cb,
                                    ad);
    }

    return ui_list;
}

void add_contact_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    appdata_s *ad = (appdata_s *) data;
    ui_show_create_page(ad);
}

void ui_show_list_page(appdata_s *ad) {
    Eina_List *contacts = contact_repository_find_all();
    Evas_Object *list   = ui_create_list(ad, contacts);
    
    Evas_Object *add_contact_btn = elm_button_add(ad->naviframe);
    elm_object_text_set(add_contact_btn, "+");
    evas_object_size_hint_min_set(add_contact_btn, 64, 64);
    evas_object_smart_callback_add(add_contact_btn, "clicked", add_contact_btn_on_click, ad);
    
    elm_naviframe_item_push(ad->naviframe, "Contacts", NULL, add_contact_btn, list, NULL);
}