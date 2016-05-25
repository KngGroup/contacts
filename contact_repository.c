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

    contacts_disconnect_on_thread();
    return result;
}

void contact_repository_list_free(Eina_List *list) {
    Eina_List *l;
    contact_s *contact;
    
    EINA_LIST_FOREACH(list, l, contact) {
        free(contact->display_name);
        free(contact->firstname);
        free(contact->lastname);
        free(contact->phone);
        free(contact);
    }
    
    eina_list_free(list);
}

void contact_repository_create(contact_s *contact) {
    int id = -1;
    contacts_record_h contact_record;
    contacts_record_h name;
    contacts_record_h number;
    
    contacts_connect_on_thread();

    contacts_record_create(_contacts_contact._uri, &contact_record);
    contacts_record_create(_contacts_name._uri, &name);

    contacts_record_set_str(name, _contacts_name.first, contact->firstname);
    contacts_record_set_str(name, _contacts_name.last, contact->lastname);
    contacts_record_add_child_record(contact_record, _contacts_contact.name, name);

    contacts_record_create(_contacts_number._uri, &number);
    contacts_record_set_str(number, _contacts_number.number, contact->phone);
    contacts_record_add_child_record(contact_record, _contacts_contact.number, number);
    
    contacts_db_insert_record(contact_record, &id);
    
    contacts_disconnect_on_thread();
}

void contact_repository_delete(int id) {
    contacts_connect_on_thread();
    contacts_db_delete_record(_contacts_person._uri, id);
    contacts_disconnect_on_thread();
}
