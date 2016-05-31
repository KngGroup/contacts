#ifndef UI_LIST_H
#define UI_LIST_H

#include "main.h"

typedef struct ui_list_page_data {
    Elm_Genlist_Item_Class *itc;
    Evas_Object *ui_list;
    Eina_List *contacts; 
    appdata_s *ad;
} ui_list_page_data_s;

void ui_show_list_page(appdata_s *ad);
void ui_list_refresh_contacts(ui_list_page_data_s *page_data);

#endif /* UI_LIST_H */