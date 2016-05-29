#include "main.h"
#include "ui_list.h"

static Evas_Event_Flags guesture_on_line_end(void *data, void *event_info) {
    Elm_Gesture_Line_Info *p = (Elm_Gesture_Line_Info *) event_info;
    dlog_print(DLOG_DEBUG, PACKAGE, "guesture_on_line_end angle=<%lf> x1,y1=<%d,%d> x2,y2=<%d,%d> tx,ty=<%u,%u> n=<%u>,\n",
            p->angle, p->momentum.x1, p->momentum.y1, p->momentum.x2, p->momentum.y2,
            p->momentum.tx, p->momentum.ty, p->momentum.n);

    if (p->angle > 170 && p->angle < 200 && p->momentum.y1 > 40 && p->momentum.y1 < 90) {
        appdata_s *ad = (appdata_s *) data;
        elm_naviframe_item_pop(ad->naviframe);

        if (NULL == elm_naviframe_top_item_get(ad->naviframe)) {
            elm_exit();
        }
    }

    return EVAS_EVENT_FLAG_ON_HOLD;
}

void create_gui(appdata_s *ad) {
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);
    
    ad->naviframe = elm_naviframe_add(ad->win);
    evas_object_size_hint_weight_set(ad->naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->naviframe);
    evas_object_show(ad->naviframe);
    
    Evas_Object *g; // Gesture layer object
    g = elm_gesture_layer_add(ad->win);
    evas_object_repeat_events_set(ad->win, EINA_TRUE);
    elm_gesture_layer_attach(g, ad->naviframe);
    evas_object_repeat_events_set(ad->naviframe, EINA_TRUE);
    
    elm_gesture_layer_cb_set(g, ELM_GESTURE_N_LINES, ELM_GESTURE_STATE_END, 
                         guesture_on_line_end, ad);
    
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
