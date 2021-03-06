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
    ui_list_page_data_s *page_data = (ui_list_page_data_s *) data;
    Elm_Object_Item *it = (Elm_Object_Item*) event_info;
    contact_s *contact  = elm_object_item_data_get(it);
    
    ui_show_contact_page(page_data->ad, contact, page_data);
    elm_genlist_item_selected_set(it, EINA_FALSE);
}

void ui_list_refresh_contacts(ui_list_page_data_s *page_data) {
    Eina_List *l;
    contact_s *contact;
    
    if (NULL != page_data->contacts) {
        contact_repository_list_free(page_data->contacts);
    }
    
    page_data->contacts = contact_repository_find_all();
    
    elm_genlist_clear(page_data->ui_list);
    
    EINA_LIST_FOREACH(page_data->contacts, l, contact) {
            elm_genlist_item_append(page_data->ui_list,
                                    page_data->itc,
                                    contact,
                                    NULL,
                                    ELM_GENLIST_ITEM_NONE,
                                    genlist_selected_cb,
                                    page_data);
    }
}

static Evas_Object *ui_create_list_page(appdata_s *ad, ui_list_page_data_s *page_data) {
    
    Evas_Object *vbox;
    
    vbox = elm_box_add(ad->win);
    elm_box_align_set(vbox, 0, 0);
    
    page_data->itc = elm_genlist_item_class_new();
    page_data->itc->item_style       = "default";
    page_data->itc->func.text_get    = genlist_text_get;
    page_data->itc->func.content_get = NULL;
    page_data->itc->func.state_get   = NULL;
    page_data->itc->func.del         = NULL;
    
    page_data->ui_list = elm_genlist_add(vbox);
    evas_object_size_hint_weight_set(page_data->ui_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(page_data->ui_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(page_data->ui_list);
    elm_box_pack_end(vbox, page_data->ui_list);
    
    page_data->contacts = NULL;
    ui_list_refresh_contacts(page_data);

    return vbox;
}

static void add_contact_btn_on_click(void *data, Evas_Object *obj, void *event_info) {
    ui_list_page_data_s *page_data = (ui_list_page_data_s *) data;
    ui_show_create_page(page_data->ad, page_data);
}

void ui_show_list_page(appdata_s *ad) {
    ui_list_page_data_s *page_data;
    page_data = malloc(sizeof(ui_list_page_data_s));
    page_data->ad = ad;
    
    Evas_Object *page = ui_create_list_page(ad, page_data);
    
    Evas_Object *add_contact_btn = elm_button_add(ad->naviframe);
    elm_object_text_set(add_contact_btn, "+");
    evas_object_size_hint_min_set(add_contact_btn, 64, 64);
    evas_object_smart_callback_add(add_contact_btn, "clicked", add_contact_btn_on_click, page_data);
    
    elm_naviframe_item_push(ad->naviframe, "Contacts", NULL, add_contact_btn, page, NULL);
}