/*
 * libidev
 * Date: Nov 2013
 * Eric Monti - esmonti at gmail dot com
 *
 * just a set of helper functions for and wrappers around the
 * libimobiledevice library -- often making gratuitous and
 * obscene use of clang blocks.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool idev_verbose=false;

#include "libidev.h"

#include <plist/plist.h>


const char *idev_idevice_strerror(idevice_error_t errnum)
{
    switch(errnum) {
        case IDEVICE_E_SUCCESS:
            return "SUCCESS";
        case IDEVICE_E_INVALID_ARG:
            return "INVALID_ARG";
        case IDEVICE_E_UNKNOWN_ERROR:
            return "UNKNOWN_ERROR";
        case IDEVICE_E_NO_DEVICE:
            return "NO_DEVICE";
        case IDEVICE_E_NOT_ENOUGH_DATA:
            return "NOT_ENOUGH_DATA";
        case IDEVICE_E_BAD_HEADER:
            return "BAD_HEADER";
        case IDEVICE_E_SSL_ERROR:
        default:
            return "UNKNOWN_EROR";
    }
}

const char *idev_lockdownd_strerror(lockdownd_error_t errnum)
{
    switch(errnum) {
        case LOCKDOWN_E_SUCCESS:
            return "SUCCESS";
        case LOCKDOWN_E_INVALID_ARG:
            return "INVALID_ARG";
        case LOCKDOWN_E_INVALID_CONF:
            return "INVALID_CONF";
        case LOCKDOWN_E_PLIST_ERROR:
            return "PLIST_ERROR";
        case LOCKDOWN_E_PAIRING_FAILED:
            return "PAIRING_FAILED";
        case LOCKDOWN_E_SSL_ERROR:
            return "SSL_ERROR";
        case LOCKDOWN_E_DICT_ERROR:
            return "DICT_ERROR";
        case LOCKDOWN_E_START_SERVICE_FAILED:
            return "START_SERVICE_FAILED";
        case LOCKDOWN_E_NOT_ENOUGH_DATA:
            return "NOT_ENOUGH_DATA";
        case LOCKDOWN_E_SET_VALUE_PROHIBITED:
            return "SET_VALUE_PROHIBITED";
        case LOCKDOWN_E_GET_VALUE_PROHIBITED:
            return "GET_VALUE_PROHIBITED";
        case LOCKDOWN_E_REMOVE_VALUE_PROHIBITED:
            return "REMOVE_VALUE_PROHIBITED";
        case LOCKDOWN_E_MUX_ERROR:
            return "MUX_ERROR";
        case LOCKDOWN_E_ACTIVATION_FAILED:
            return "ACTIVATION_FAILED";
        case LOCKDOWN_E_PASSWORD_PROTECTED:
            return "PASSWORD_PROTECTED";
        case LOCKDOWN_E_NO_RUNNING_SESSION:
            return "NO_RUNNING_SESSION";
        case LOCKDOWN_E_INVALID_HOST_ID:
            return "INVALID_HOST_ID";
        case LOCKDOWN_E_INVALID_SERVICE:
            return "INVALID_SERVICE";
        case LOCKDOWN_E_INVALID_ACTIVATION_RECORD:
            return "INVALID_ACTIVATION_RECORD";
        case LOCKDOWN_E_UNKNOWN_ERROR:
        default:
            return "UNKNOWN_EROR";
    }
}

const char *idev_file_relay_strerror(file_relay_error_t errnum) {
    switch(errnum) {
        case FILE_RELAY_E_SUCCESS:
            return "SUCCESS";
        case FILE_RELAY_E_INVALID_ARG:
            return "INVALID_ARG";
        case FILE_RELAY_E_PLIST_ERROR:
            return "PLIST_ERROR";
        case FILE_RELAY_E_MUX_ERROR:
            return "MUX_ERROR";
        case FILE_RELAY_E_INVALID_SOURCE:
            return "INVALID_SOURCE";
        case FILE_RELAY_E_STAGING_EMPTY:
            return "STAGING_EMPTY";
        case FILE_RELAY_E_UNKNOWN_ERROR:
        default:
            return "UNKNOWN_EROR";
    }
}

const char *idev_instproxy_strerror(instproxy_error_t errnum)
{
    switch(errnum) {
        case INSTPROXY_E_SUCCESS:
            return "SUCCESS";
        case INSTPROXY_E_INVALID_ARG:
            return "INVALID_ARG";
        case INSTPROXY_E_PLIST_ERROR:
            return "PLIST_ERROR";
        case INSTPROXY_E_CONN_FAILED:
            return "CONN_FAILED";
        case INSTPROXY_E_OP_IN_PROGRESS:
            return "OP_IN_PROGRESS";
        case INSTPROXY_E_OP_FAILED:
            return "OP_FAILED";
        case INSTPROXY_E_UNKNOWN_ERROR:
        default:
            return "UNKNOWN_EROR";
    }
}

const char *idev_afc_strerror(afc_error_t errnum)
{
    switch(errnum) {
        case AFC_E_SUCCESS:
            return "SUCCESS";
        case AFC_E_UNKNOWN_ERROR:
            return "UNKNOWN_ERROR";
        case AFC_E_OP_HEADER_INVALID:
            return "OP_HEADER_INVALID";
        case AFC_E_NO_RESOURCES:
            return "NO_RESOURCES";
        case AFC_E_READ_ERROR:
            return "READ_ERROR";
        case AFC_E_WRITE_ERROR:
            return "WRITE_ERROR";
        case AFC_E_UNKNOWN_PACKET_TYPE:
            return "UNKNOWN_PACKET_TYPE";
        case AFC_E_INVALID_ARG:
            return "INVALID_ARG";
        case AFC_E_OBJECT_NOT_FOUND:
            return "OBJECT_NOT_FOUND";
        case AFC_E_OBJECT_IS_DIR:
            return "OBJECT_IS_DIR";
        case AFC_E_PERM_DENIED:
            return "PERM_DENIED";
        case AFC_E_SERVICE_NOT_CONNECTED:
            return "SERVICE_NOT_CONNECTED";
        case AFC_E_OP_TIMEOUT:
            return "OP_TIMEOUT";
        case AFC_E_TOO_MUCH_DATA:
            return "TOO_MUCH_DATA";
        case AFC_E_END_OF_DATA:
            return "END_OF_DATA";
        case AFC_E_OP_NOT_SUPPORTED:
            return "OP_NOT_SUPPORTED";
        case AFC_E_OBJECT_EXISTS:
            return "OBJECT_EXISTS";
        case AFC_E_OBJECT_BUSY:
            return "OBJECT_BUSY";
        case AFC_E_NO_SPACE_LEFT:
            return "NO_SPACE_LEFT";
        case AFC_E_OP_WOULD_BLOCK:
            return "OP_WOULD_BLOCK";
        case AFC_E_IO_ERROR:
            return "IO_ERROR";
        case AFC_E_OP_INTERRUPTED:
            return "OP_INTERRUPTED";
        case AFC_E_OP_IN_PROGRESS:
            return "OP_IN_PROGRESS";
        case AFC_E_INTERNAL_ERROR:
            return "INTERNAL_ERROR";
        case AFC_E_MUX_ERROR:
            return "MUX_ERROR";
        case AFC_E_NO_MEM:
            return "NO_MEM";
        case AFC_E_NOT_ENOUGH_DATA:
            return "NOT_ENOUGH_DATA";
        case AFC_E_DIR_NOT_EMPTY:
            return "DIR_NOT_EMPTY";
        default:
            return "UNKNOWN_EROR";
    }
}

const char *idev_house_arrest_strerror(house_arrest_error_t errnum)
{
    switch(errnum) {
        case HOUSE_ARREST_E_SUCCESS:
            return "SUCCESS";
        case HOUSE_ARREST_E_INVALID_ARG:
            return "INVALID_ARG";
        case HOUSE_ARREST_E_PLIST_ERROR:
            return "PLIST_ERROR";
        case HOUSE_ARREST_E_CONN_FAILED:
            return "CONN_FAILED";
        case HOUSE_ARREST_E_INVALID_MODE:
            return "INVALID_MODE";
        case HOUSE_ARREST_E_UNKNOWN_ERROR:
        default:
            return "UNKNOWN_EROR";
    }
}

// note: clientname may be null in which case a default value of "idevtool" is used
int idev_lockdownd_client(char *clientname, char *udid, int(^callback)(idevice_t idev, lockdownd_client_t client))
{
    int ret=EXIT_FAILURE;
    idevice_t idev = NULL;

    if (!clientname)
        clientname = "idevtool";

    idevice_error_t ierr=idevice_new(&idev, udid);

    if (ierr == IDEVICE_E_SUCCESS && idev) {
        lockdownd_client_t client = NULL;
        lockdownd_error_t ldret = lockdownd_client_new_with_handshake(idev, &client, clientname);

        if (ldret == LOCKDOWN_E_SUCCESS && client) {
            ret = callback(idev, client);
        } else {
            fprintf(stderr, "Error: Can't connect to lockdownd: %s.\n", idev_lockdownd_strerror(ldret));
        }

        if (client)
            lockdownd_client_free(client);

    } else if (ierr == IDEVICE_E_NO_DEVICE) {
        fprintf(stderr, "Error: No device found -- Is it plugged in?\n");
    } else {
        fprintf(stderr, "Error: Cannot connect to device: %s\n", idev_idevice_strerror(ierr));
    }

    if (idev)
        idevice_free(idev);

    return ret;
}

// starts a remote lockdownd service instance and returns an initialized lockdownd_service_descriptor_t
int idev_lockdownd_start_service (
        char *progname,
        char *udid,
        char *servicename,
        int(^block)(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc) )
{
    return idev_lockdownd_client(progname, udid,
                                 ^int(idevice_t idev, lockdownd_client_t client)
    {
        int ret=EXIT_FAILURE;

        if (idev_verbose) fprintf(stderr, "[debug] starting '%s' lockdownd service\n", servicename);

        lockdownd_service_descriptor_t ldsvc = NULL;
        lockdownd_error_t ldret = lockdownd_start_service(client, servicename, &ldsvc);

        if ((ldret == LOCKDOWN_E_SUCCESS) && ldsvc) {

            block(idev, client, ldsvc);

        } else {
            fprintf(stderr, "Error: could not start service: %s", servicename);
        }

        if (ldsvc) lockdownd_service_descriptor_free(ldsvc);

        return ret;

    });

}

// connects to a lockdownd service by name and returns an initialized idevice_connection_t
int idev_lockdownd_connect_service (
        char *progname,
        char *udid,
        char *servicename,
        int(^block)(idevice_t idev, lockdownd_service_descriptor_t ldsvc, idevice_connection_t con) )
{
    return idev_lockdownd_start_service(progname, udid, servicename,
            ^int(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc)
    {
        int ret=EXIT_FAILURE;

        if (idev_verbose) fprintf(stderr, "[debug] connecting to service: %s\n", servicename);

        idevice_connection_t con=NULL;
        idevice_error_t ierr = idevice_connect(idev, ldsvc->port, &con);
        if (ierr == IDEVICE_E_SUCCESS && con) {
            if (idev_verbose) fprintf(stderr, "[debug] successfully connected to %s\n", servicename);

            ret = block(idev, ldsvc, con);

        } else {
            fprintf(stderr, "Error: could not connect to lockdownd service");
        }

        if (con) idevice_disconnect(con);

        return ret;
    });
}


// Retrieve the device local path to the app binary based on its display name or bundle id
char * idev_get_app_path(idevice_t idevice, lockdownd_client_t lockd, const char *app)
{
    char *ret=NULL;
    char *path_str=NULL;
    char *exec_str=NULL;

    if (idev_verbose) { fprintf(stderr, "[debug]: looking up exec path for %s\n", app); }

    lockdownd_service_descriptor_t ldsvc = NULL;
    if ((lockdownd_start_service(lockd, "com.apple.mobile.installation_proxy", &ldsvc) == LOCKDOWN_E_SUCCESS) && ldsvc) {

        instproxy_client_t ipc = NULL;
        if (instproxy_client_new(idevice, ldsvc, &ipc) == INSTPROXY_E_SUCCESS) {

            plist_t client_opts = instproxy_client_options_new();

            plist_t apps = NULL;
            instproxy_error_t err = instproxy_browse(ipc, client_opts, &apps);
            if (err == INSTPROXY_E_SUCCESS) {
                plist_t app_found = NULL;

                app_found = NULL;
                uint32_t i;
                for (i = 0; i < plist_array_get_size(apps); i++) {
                    char *appid_str = NULL;
                    char *name_str = NULL;

                    plist_t app_info = plist_array_get_item(apps, i);
                    plist_t appid_p = plist_dict_get_item(app_info, "CFBundleIdentifier");
                    if (appid_p)
                        plist_get_string_val(appid_p, &appid_str);

                    plist_t disp_p = plist_dict_get_item(app_info, "CFBundleDisplayName");
                    if (disp_p)
                        plist_get_string_val(disp_p, &name_str);

                    if (appid_str && strcmp(app, appid_str) == 0) {
                        if (!app_found)
                            app_found = app_info;
                        else
                            fprintf(stderr, "Error: ambigous bundle ID: %s\n", app);
                    }

                    if (name_str && strcmp(app, name_str) == 0) {
                        if (!app_found)
                            app_found = app_info;
                        else
                            fprintf(stderr, "Error: ambigous app name: %s\n", app);
                    }

                    if (appid_str)
                        free(appid_str);

                    if (name_str)
                        free(name_str);
                }

                if (app_found) {
                    plist_t path_p = plist_dict_get_item(app_found, "Path");
                    if (path_p)
                        plist_get_string_val(path_p, &path_str);


                    plist_t exe_p = plist_dict_get_item(app_found, "CFBundleExecutable");
                    if (exe_p)
                        plist_get_string_val(exe_p, &exec_str);

                } else {
                    fprintf(stderr, "Error: No app found with name or bundle id: %s\n", app);
                }

            } else {
                fprintf(stderr, "Error: Unable to browse applications. Error code %s\n", idev_instproxy_strerror(err));
            }

            if (apps)
                plist_free(apps);

            if (client_opts)
                plist_free(client_opts);

        } else {
            fprintf(stderr, "Error: Could not connect to installation_proxy!\n");
        }

        if (ipc)
            instproxy_client_free(ipc);

    } else {
        fprintf(stderr, "Error: Could not start com.apple.mobile.installation_proxy!\n");
    }

    if (ldsvc)
        lockdownd_service_descriptor_free(ldsvc);


    if (path_str) {
        if (exec_str) {
            asprintf(&ret, "%s/%s", path_str, exec_str);
            if (idev_verbose) { fprintf(stderr, "[debug]: found exec path: %s\n", ret); }

        } else {
            fprintf(stderr, "Error: bundle executable not found\n");
        }

    } else {
        fprintf(stderr, "Error: app path not found\n");
    }

    return ret;
}


#pragma mark - AFC helpers


#define AFC_SERVICE_NAME "com.apple.afc"
#define AFC2_SERVICE_NAME "com.apple.afc2"
#define HOUSE_ARREST_SERVICE_NAME "com.apple.mobile.house_arrest"


int idev_afc_client_ex(
        char *clientname,
        char *udid,
        char *afc_servicename,
        int(^block)(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc, afc_client_t afc) )
{
    return idev_lockdownd_start_service (
        clientname,
        udid,
        afc_servicename,
        ^int(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc)
    {
        int ret=EXIT_FAILURE;
        afc_client_t afc = NULL;
        afc_error_t afc_err = afc_client_new(idev, ldsvc, &afc);

        if (afc_err == AFC_E_SUCCESS && afc) {

            ret = block(idev, client, ldsvc, afc);

        } else {
            fprintf(stderr, "Error: unable to create afc client: %s\n", idev_afc_strerror(afc_err));
        }

        if (afc) afc_client_free(afc);

        return ret;
    });
}


int idev_afc_client(char *clientname, char *udid, bool root, int(^block)(afc_client_t afc))
{
    return idev_afc_client_ex (
            clientname,
            udid,
            ((root)? AFC2_SERVICE_NAME : AFC_SERVICE_NAME),
            ^int(idevice_t i, lockdownd_client_t c, lockdownd_service_descriptor_t l, afc_client_t afc) 
    {
        return block(afc);
    });
}

int idev_afc_app_client(char *clientname, char *udid, char *appid, int(^block)(afc_client_t afc))
{
    return idev_lockdownd_client(clientname, udid, ^int(idevice_t idev, lockdownd_client_t client) {
        int ret = EXIT_FAILURE;

        lockdownd_service_descriptor_t ldsvc=NULL;
        lockdownd_error_t lret = lockdownd_start_service(client, HOUSE_ARREST_SERVICE_NAME, &ldsvc);

        if (lret == LOCKDOWN_E_SUCCESS && ldsvc) {

            house_arrest_client_t ha_client=NULL;
            house_arrest_error_t ha_err = house_arrest_client_new(idev, ldsvc, &ha_client);

            if (ha_err == HOUSE_ARREST_E_SUCCESS && ha_client) {

                ha_err = house_arrest_send_command(ha_client, "VendContainer", appid);

                if (ha_err == HOUSE_ARREST_E_SUCCESS) {
                    plist_t dict = NULL;
                    ha_err = house_arrest_get_result(ha_client, &dict);

                    if (ha_err == HOUSE_ARREST_E_SUCCESS && dict) {
                        plist_t errnode = plist_dict_get_item(dict, "Error");

                        if (!errnode) {
                            afc_client_t afc=NULL;
                            afc_error_t afc_err = afc_client_new_from_house_arrest_client(ha_client, &afc);

                            if (afc_err == AFC_E_SUCCESS && afc) {

                                ret = block(afc);

                            } else {
                                fprintf(stderr, "Error: could not get afc client from house arrest: %s\n", idev_afc_strerror(afc_err));
                            }

                            if (afc)
                                afc_client_free(afc);

                        } else {
                            char *str = NULL;
                            plist_get_string_val(errnode, &str);
                            fprintf(stderr, "Error: house_arrest service responded: %s\n", str);
                            if (str)
                                free(str);
                        }
                    } else {
                        fprintf(stderr, "Error: Could not get result form house_arrest service: %s\n",
                                idev_house_arrest_strerror(ha_err));
                    }

                    if (dict)
                        plist_free(dict);

                } else {
                    fprintf(stderr, "Error: Could not send VendContainer command with argument:%s - %s\n", 
                            appid, idev_house_arrest_strerror(ha_err));
                }

            } else {
                fprintf(stderr, "Error: Unable to create house arrest client: %s\n", idev_house_arrest_strerror(ha_err));
            }

            if (ha_client)
                house_arrest_client_free(ha_client);

        } else {
            fprintf(stderr, "Error: unable to start service: %s - %s\n", HOUSE_ARREST_SERVICE_NAME, idev_lockdownd_strerror(lret));
        }

        return ret;
    });
}

