#include "elm_ext.h"

Evas_Object *elm_ext_spacer_add(Evas_Object *parent) {
    Evas_Object *spacer;
    
    spacer = evas_object_rectangle_add(evas_object_evas_get(parent));
    evas_object_size_hint_min_set(spacer, 0, 15);
    evas_object_color_set(spacer, 0, 0, 0, 0);
    
    return spacer;
}

Evas_Object *elm_ext_separator_add(Evas_Object *parent) {
    Evas_Object *separator;
    
    separator = evas_object_rectangle_add(evas_object_evas_get(parent));
    evas_object_color_set(separator, 60, 60, 60, 255);
    evas_object_size_hint_min_set(separator, 0, 1);
    evas_object_size_hint_weight_set(separator, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(separator, EVAS_HINT_FILL, 0);
    
    return separator;
}

Evas_Object *elm_ext_vbox_add_separator(Evas_Object *box) {
    Evas_Object *spacer, *separator;
    
    Evas_Object *vbox = elm_box_add(box);
    evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    
    spacer = elm_ext_spacer_add(vbox);
    elm_box_pack_end(vbox, spacer);
    
    separator = elm_ext_separator_add(vbox);
    elm_box_pack_end(vbox, separator);
    evas_object_show(separator);
    
    spacer = elm_ext_spacer_add(vbox);
    elm_box_pack_end(vbox, spacer);
    
    evas_object_show(vbox);
    elm_box_pack_end(box, vbox);
    
    return vbox;
}