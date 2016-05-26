cmake_minimum_required (VERSION 3.0)
project (Contacts)

set (Contacts_PACKAGE Contacts)
set (Contacts_VERSION_MAJOR 0)
set (Contacts_VERSION_MINOR 0)
set (Contacts_VERSION_PATCH 1)
set (Contacts_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file (
  "${Contacts_INCLUDE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )


set(CMAKE_FIND_PATH /home/bykov/tau)
set(TIZEN_PATH /home/bykov/tau)
set(TIZEN_INCLUDE_PATH "${TIZEN_PATH}/usr/include")

include_directories(
  "${PROJECT_BINARY_DIR}"
  "${Contacts_INCLUDE_DIR}"
  "${TIZEN_INCLUDE_PATH}/dlog"
  "${TIZEN_INCLUDE_PATH}/elementary-1"
  "${TIZEN_INCLUDE_PATH}/eina-1"
  "${TIZEN_INCLUDE_PATH}/eina-1/eina"
  "${TIZEN_INCLUDE_PATH}/e_dbus-1"
  "${TIZEN_INCLUDE_PATH}/ecore-1"
  "${TIZEN_INCLUDE_PATH}/edje-1"
  "${TIZEN_INCLUDE_PATH}/efreet-1"
  "${TIZEN_INCLUDE_PATH}/eio-1"
  "${TIZEN_INCLUDE_PATH}/eet-1"
  "${TIZEN_INCLUDE_PATH}/evas-1"
  "${TIZEN_INCLUDE_PATH}/dbus-1.0"
  "${TIZEN_INCLUDE_PATH}/ethumb-1"
  "${TIZEN_INCLUDE_PATH}/contacts-svc"
  "${TIZEN_PATH}/usr/lib/dbus-1.0/include"
)


find_library(LIBDLOG NAMES dlog)
find_library(LIBELEMENTARY NAMES elementary)
find_library(LIBEVAS NAMES evas)
find_library(LIBCONTACTSSERVICE NAMES contacts-service2)
find_library(LIBEINA NAMES eina)
find_library(LIBEDJE NAMES edje)

add_executable(contacts main.c ui_list.c ui_show.c ui_create.c contact_repository.c)
target_link_libraries(
  contacts
  "${LIBDLOG}"
  "${LIBELEMENTARY}"
  "${LIBEVAS}"
  "${LIBCONTACTSSERVICE}"
  "${LIBEINA}"
  "${LIBEDJE}"
)
