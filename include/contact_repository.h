#ifndef CONTACT_REPOSITORY_H
#define CONTACT_REPOSITORY_H

#include "main.h"

typedef struct contact {
    int id;
    char *display_name;
} contact_s;

Eina_List *contact_repository_find_all();
void contact_repository_list_free(Eina_List *list);

#endif /* CONTACT_REPOSITORY_H */