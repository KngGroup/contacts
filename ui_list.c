#include "ui_list.h"
#include "ui_show.h"

static char *genlist_text_get(void *data, Evas_Object *obj, const char *part) {
    if (strcmp(part, "elm.text") == 0) {
        char *name = (char *) data;

        return strdup(name);
    } else {
        return NULL;
    }
}

static Evas_Object *genlist_content_get(void *data, Evas_Object *obj, const char *part) {
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

static void genlist_selected_cb(void *data, Evas_Object *obj, void *event_info) {
    appdata_s *ad = data;

    Elm_Object_Item *it = (Elm_Object_Item*) event_info;

    char *name = strdup(elm_object_item_data_get(it));
    ui_show_contact_page(ad, name);
}

static Evas_Object *ui_create_list(appdata_s *ad) {
    Elm_Genlist_Item_Class *itc;
    Evas_Object *ui_list;

    char *contact_display_name;
    contacts_record_h contact_record;
    contacts_list_h contact_list;

    itc = elm_genlist_item_class_new();
    itc->item_style       = "default";
    itc->func.text_get    = genlist_text_get;
    itc->func.content_get = genlist_content_get;
    itc->func.state_get   = NULL;
    itc->func.del         = NULL;

    ui_list = elm_genlist_add(ad->win);

    contacts_connect_on_thread();
    contacts_db_get_all_records(_contacts_person._uri, 0, 0, &contact_list);
    while (contacts_list_get_current_record_p(contact_list, &contact_record) == CONTACTS_ERROR_NONE) {
        contacts_record_get_str_p(contact_record, _contacts_person.display_name, &contact_display_name);
        elm_genlist_item_append(ui_list,
                                itc,
                                contact_display_name,
                                NULL,
                                ELM_GENLIST_ITEM_NONE,
                                genlist_selected_cb,
                                ad);

        contacts_list_next(contact_list);
    }

    return ui_list;
}

void ui_show_list_page(appdata_s *ad) {
    Evas_Object *list = ui_create_list(ad);
    elm_naviframe_item_push(ad->naviframe, "Contacts", NULL, NULL, list, NULL);
}