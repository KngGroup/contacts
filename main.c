#include "main.h"
#include "ui_list.h"

static Eina_Bool _key_down_cb(void *data, int type, void *ev) {
    Ecore_Event_Key *event = ev;
    
    if (!strcmp("XF86Stop", event->key)) {
        appdata_s *ad = (appdata_s *) data;
        elm_naviframe_item_pop(ad->naviframe);

        if (NULL == elm_naviframe_top_item_get(ad->naviframe)) {
            elm_exit();
        }
        
        return ECORE_CALLBACK_DONE;
    }
    
    return ECORE_CALLBACK_PASS_ON;
}

void create_gui(appdata_s *ad) {
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);
    
    ad->conformant = elm_conformant_add(ad->win);
    evas_object_size_hint_weight_set(ad->conformant, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->conformant);
    evas_object_show(ad->conformant);
    
    ad->naviframe = elm_naviframe_add(ad->win);
    elm_object_content_set(ad->conformant, ad->naviframe);
    evas_object_show(ad->naviframe);
    
    ui_show_list_page(ad);
    evas_object_show(ad->win);
    
    ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, _key_down_cb, ad);
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
