#include "ui_create.h"

create_test_contacts() {
    contacts_record_h contact;
    contacts_record_h name;
    contacts_record_h number;
    int i;
    contacts_connect_on_thread();

    for (i = 0; i < 10; i++) {
        int id = -1;

        contacts_record_create(_contacts_contact._uri, &contact);
        contacts_record_create(_contacts_name._uri, &name);

        contacts_record_set_str(name, _contacts_name.first, "John");
        contacts_record_set_str(name, _contacts_name.last, "Smith");
        contacts_record_add_child_record(contact, _contacts_contact.name, name);

        contacts_record_create(_contacts_number._uri, &number);
        contacts_record_set_str(number, _contacts_number.number, "+71111111111");
        contacts_record_add_child_record(contact, _contacts_contact.number, number);

        contacts_db_insert_record(contact, &id);
    }
}