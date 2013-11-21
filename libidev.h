/*
 * libidev
 * Date: Nov 2013
 * Eric Monti - esmonti at gmail dot com
 *
 * just a set of helper functions for and wrappers around the
 * libimobiledevice library -- often making gratuitous and
 * obscene use of clang blocks.
 */


#ifndef _libidev_h
#define _libidev_h

#include <stdbool.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/file_relay.h>
#include <libimobiledevice/installation_proxy.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/house_arrest.h>

extern bool idev_verbose;

const char *idev_idevice_strerror(idevice_error_t errnum);

const char *idev_lockdownd_strerror(lockdownd_error_t errnum);

const char *idev_file_relay_strerror(file_relay_error_t errnum);

const char *idev_instproxy_strerror(instproxy_error_t errnum);

const char *idev_afc_strerror(afc_error_t errnum);

char * idev_get_app_path(idevice_t idevice, lockdownd_client_t lockd, const char *app);

int idev_lockdownd_client (
        char *clientname,
        char *udid,
        int(^callback)(idevice_t idev, lockdownd_client_t client) );

int idev_lockdownd_start_service (
        char *progname,
        char *udid,
        char *servicename,
        int(^block)(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc) );

int idev_lockdownd_connect_service (
        char *progname,
        char *udid,
        char *servicename,
        int(^block)(idevice_t idev, lockdownd_service_descriptor_t ldsvc, idevice_connection_t con) );

int idev_afc_client_ex (
        char *clientname,
        char *udid,
        char *afc_servicename,
        int(^block)(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc, afc_client_t afc) );

int idev_afc_client (
        char *clientname,
        char *udid,
        bool root,
        int(^block)(afc_client_t afc) );

int idev_afc_app_client(
        char *clientname,
        char *udid,
        char *appid,
        int(^block)(afc_client_t afc) );

#endif // _libidev_h
