#ifndef CONTACT_REPOSITORY_H
#define CONTACT_REPOSITORY_H

#include "main.h"

typedef struct contact {
    int id;
    char *display_name;
    char *firstname;
    char *lastname;
    char *default_phone;
    Eina_List *phone_numbers;
    Eina_List *emails;
    char *note;
} contact_s;

typedef struct phone {
    char *number;
    int type;
} phone_s;

contact_s *contact_repository_contact_s_new();
void contact_repository_contact_s_free(contact_s * contact);

phone_s *contact_repository_phone_s_new();
void contact_repository_phone_s_free(phone_s *phone);

Eina_List *contact_repository_find_all();
void contact_repository_list_free(Eina_List *list);
void contact_repository_create(contact_s *contact);
void contact_repository_delete(int id);
char *contact_repository_get_phone_by_id(int id);

#endif /* CONTACT_REPOSITORY_H */