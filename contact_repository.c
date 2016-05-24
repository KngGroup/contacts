#include <contact_repository.h>

static contact_s *hydrate(contacts_record_h record) {
    contact_s *contact;
    contact = malloc(sizeof (contact_s));
    memset(contact, 0x0, sizeof (contact_s));

    contacts_record_get_int(record, _contacts_person.id, &contact->id);
    contacts_record_get_str(record, _contacts_person.display_name, &contact->display_name);

    return contact;
}

Eina_List *contact_repository_find_all() {
    contacts_record_h contact_record;
    contacts_list_h contact_list;
    Eina_List *result = NULL;

    contacts_connect_on_thread();
    contacts_db_get_all_records(_contacts_person._uri, 0, 0, &contact_list);

    while (contacts_list_get_current_record_p(contact_list, &contact_record) == CONTACTS_ERROR_NONE) {
        contact_s *contact = hydrate(contact_record);
        result = eina_list_append(result, contact);
        contacts_list_next(contact_list);
    }

    return result;
}

void contact_repository_list_free(Eina_List *list) {
    Eina_List *l;
    contact_s *contact;
    
    EINA_LIST_FOREACH(list, l, contact) {
        free(contact->display_name);
        free(contact);
    }
    
    eina_list_free(list);
}

