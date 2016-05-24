#include "main.h"
#include "ui_list.h"

void create_gui(appdata_s *ad) {
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);

    ad->naviframe = elm_naviframe_add(ad->win);
    evas_object_size_hint_weight_set(ad->naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->naviframe);
    evas_object_show(ad->naviframe);
    
    ui_show_list_page(ad);
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
