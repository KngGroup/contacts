#include <contact_repository.h>

contact_s *contact_repository_contact_s_new() {
    contact_s *contact;
    contact = malloc(sizeof (contact_s));
    memset(contact, 0x0, sizeof (contact_s));
    
    return contact;
}

void contact_repository_contact_s_free(contact_s * contact) {
    Eina_List *l;
    char *phone_number;
    char *email;
    
    free(contact->display_name);
    free(contact->firstname);
    free(contact->lastname);
    free(contact->default_phone);
    free(contact->note);

    EINA_LIST_FOREACH(contact->phone_numbers, l, phone_number) {
        free(phone_number);
    }
    
    EINA_LIST_FOREACH(contact->emails, l, email) {
        free(email);
    }

    eina_list_free(contact->phone_numbers);
    eina_list_free(contact->emails);
    free(contact);
}

phone_s *contact_repository_phone_s_new() {
    phone_s *phone;
    phone = malloc(sizeof (phone_s));
    memset(phone, 0x0, sizeof (phone_s));
    
    return phone;
}

void contact_repository_phone_s_free(phone_s *phone) {
    free(phone->number);
}

static contact_s *hydrate(contacts_record_h record) {
    contact_s *contact = contact_repository_contact_s_new();

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
    
    contacts_list_destroy(contact_list, EINA_TRUE);

    contacts_disconnect_on_thread();
    return result;
}

void contact_repository_list_free(Eina_List *list) {
    Eina_List *l;
    contact_s *contact;
    
    EINA_LIST_FOREACH(list, l, contact) {
        contact_repository_contact_s_free(contact);
    }
    
    eina_list_free(list);
}

void contact_repository_create(contact_s *contact) {
    int id = -1;
    Eina_List *l;
    phone_s *phone;
    char *email;
    
    contacts_record_h contact_record;
    contacts_record_h name;
    contacts_record_h number;
    contacts_record_h email_record;
    contacts_record_h note_record;
    
    contacts_connect_on_thread();

    contacts_record_create(_contacts_contact._uri, &contact_record);
    
    contacts_record_create(_contacts_name._uri, &name);
    contacts_record_set_str(name, _contacts_name.first, contact->firstname);
    contacts_record_set_str(name, _contacts_name.last, contact->lastname);
    contacts_record_add_child_record(contact_record, _contacts_contact.name, name);

    EINA_LIST_FOREACH(contact->phone_numbers, l, phone) {
        contacts_record_create(_contacts_number._uri, &number);
        
        contacts_record_set_str(number, _contacts_number.number, phone->number);
        contacts_record_set_int(number, _contacts_number.type, phone->type);
        
        contacts_record_add_child_record(contact_record, _contacts_contact.number, number);
    }

    EINA_LIST_FOREACH(contact->emails, l, email) {
        contacts_record_create(_contacts_email._uri, &email_record);
        contacts_record_set_str(email_record, _contacts_email.email, email);
        contacts_record_add_child_record(contact_record, _contacts_contact.email, email_record);
    }
    
    contacts_record_create(_contacts_note._uri, &note_record);
    contacts_record_set_str(name, _contacts_note.note, contact->note);
    contacts_record_add_child_record(contact_record, _contacts_contact.note, note_record);
    
    contacts_db_insert_record(contact_record, &id);
    contacts_record_destroy(contact_record, TRUE);
    
    contacts_disconnect_on_thread();
}

void contact_repository_delete(int id) {
    contacts_connect_on_thread();
    contacts_db_delete_record(_contacts_person._uri, id);
    contacts_disconnect_on_thread();
}

char *contact_repository_get_phone_by_id(int id) {
    contacts_query_h query = NULL;
    contacts_filter_h filter = NULL;
    contacts_list_h list = NULL;
    contacts_record_h record_person_number = NULL;
    char *phone;
    
    contacts_connect_on_thread();
    
    contacts_query_create(_contacts_person_number._uri, &query);
    contacts_filter_create(_contacts_person_number._uri, &filter);
    contacts_filter_add_int(filter, _contacts_person_number.person_id, CONTACTS_MATCH_EQUAL, id);
    contacts_filter_add_operator(filter, CONTACTS_FILTER_OPERATOR_AND);
    contacts_filter_add_bool(filter, _contacts_person_number.is_primary_default, EINA_TRUE);
    contacts_query_set_filter(query, filter);

    // Run the query: Retrieve the phone number records containing the default phone number
    contacts_db_get_records_with_query(query, 0, 0, &list);

    // Retrieve the current record from the query list
    contacts_list_get_current_record_p(list, &record_person_number);

    // Retrieve the phone number from the phone number record
    contacts_record_get_str(record_person_number, _contacts_person_number.number, &phone);

    // Destroy the list, filter, and query handles and release all their resources
    contacts_list_destroy(list, EINA_TRUE);
    contacts_filter_destroy(filter);
    contacts_query_destroy(query);

    contacts_disconnect_on_thread();
    
    return phone;
}

void contact_repository_update(contact_s *contact) {
    contacts_record_h contact_record;
    contacts_record_h name;
    contacts_record_h phone;

    contacts_connect_on_thread();
    contacts_db_get_record(_contacts_contact._uri, contact->id, &contact_record);
    
    contacts_record_get_child_record_at_p(contact_record, _contacts_contact.name, 0, &name);
    contacts_record_set_str(name, _contacts_name.first, contact->firstname);
    contacts_record_set_str(name, _contacts_name.last, contact->lastname);
    
    contacts_record_get_child_record_at_p(contact_record, _contacts_contact.number, 0, &phone);
    contacts_record_set_str(phone, _contacts_number.number, contact->default_phone);
    
    contacts_db_update_record(contact_record);
    
    contacts_disconnect_on_thread();
}