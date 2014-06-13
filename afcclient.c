/*
 * afcclient
 * Date: Nov 2013
 * Eric Monti - esmonti at gmail dot com
 *
 * A simple CLI interface to AFC via libimobiledevice.
 *
 * Build with 'make'
 *
 * run "afcclient -h" for usage.
 *
 */

#ifdef __linux
  #include <limits.h>
#endif

#ifdef __APPLE__
  #include <sys/syslimits.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "libidev.h"


#define CHUNKSZ 8192

#pragma mark - AFC Implementation Utility Functions

char *progname;
void usage(FILE *outf);

bool is_dir(char *path)
{
    struct stat s;
    return (stat(path, &s) == 0 && s.st_mode & S_IFDIR);
}

int dump_afc_device_info(afc_client_t afc)
{
    int ret=EXIT_FAILURE;

    char **infos=NULL;
    afc_error_t err=afc_get_device_info(afc, &infos);
    if (err == AFC_E_SUCCESS && infos) {
        int i;
        printf("AFC Device Info: -");
        for (i=0; infos[i]; i++)
            printf("%c%s", ((i%2)? ':' : ' '), infos[i]);

        printf("\n");
        ret = EXIT_SUCCESS;

    } else {
        fprintf(stderr, "Error: afc get device info failed: %s\n", idev_afc_strerror(err));
    }

    if (infos)
        idevice_device_list_free(infos);

    return ret;
}

int dump_afc_file_info(afc_client_t afc, const char *path)
{
    int i, ret=EXIT_FAILURE;

    char **infolist=NULL;
    afc_error_t err = afc_get_file_info(afc, path, &infolist);

    if (err == AFC_E_SUCCESS && infolist) {
        for(i=0; infolist[i]; i++)
            printf("%c%s", ((i%2)? '=' : ' '), infolist[i]);

        printf("\t%s\n", path);
        ret=EXIT_SUCCESS;

    } else {
        fprintf(stderr, "Error: info error for path: %s - %s\n", path, idev_afc_strerror(err));
    }

    if (infolist)
        idevice_device_list_free(infolist);

    return ret;
}

int dump_afc_list_path(afc_client_t afc, const char *path)
{
    int ret=EXIT_FAILURE;

    char **list=NULL;

    if (idev_verbose)
        fprintf(stderr, "[debug] reading afc directory contents at \"%s\"\n", path);

    afc_error_t err = afc_read_directory(afc, path, &list);

    if (err == AFC_E_SUCCESS && list) {
        int i;
        printf("AFC Device Listing path=\"%s\":\n", path);
        for (i=0; list[i]; i++) {
            char tpath[PATH_MAX], *lpath;
            if (!strcmp(path, "")) {
                lpath=list[i];
            } else {
                if (path[strlen(path)-1]=='/') {
                    snprintf(tpath, PATH_MAX-1, "%s%s", path, list[i]);
                } else {
                    snprintf(tpath, PATH_MAX-1, "%s/%s", path, list[i]);
                }
                lpath = tpath;
            }

            dump_afc_file_info(afc, lpath);
        }

        ret=EXIT_SUCCESS;
    } else if (err == AFC_E_READ_ERROR) { // fall-back to doing a file info request, incase its a file
        if (idev_verbose)
            fprintf(stderr, "[debug] directory read error -- falling back to file info at %s\n", path);

        ret = dump_afc_file_info(afc, path);
    } else {
        fprintf(stderr, "Error: afc list \"%s\" failed: %s\n", path, idev_afc_strerror(err));
    }

    if (list)
        free(list);

    return ret;
}


int dump_afc_path(afc_client_t afc, const char *path, FILE *outf)
{
    int ret=EXIT_FAILURE;

    uint64_t handle=0;

    if (idev_verbose)
        fprintf(stderr, "[debug] creating afc file connection to %s\n", path);

    afc_error_t err = afc_file_open(afc, path, AFC_FOPEN_RDONLY, &handle);

    if (err == AFC_E_SUCCESS) {
        char buf[CHUNKSZ];
        uint32_t bytes_read=0;

        while((err=afc_file_read(afc, handle, buf, CHUNKSZ, &bytes_read)) == AFC_E_SUCCESS && bytes_read > 0) {
            fwrite(buf, 1, bytes_read, outf);
        }

        if (err)
            fprintf(stderr, "Error: Encountered error while reading %s: %s\n", path, idev_afc_strerror(err));
        else
            ret=EXIT_SUCCESS;

        afc_file_close(afc, handle);
    } else {
        fprintf(stderr, "Error: afc open file %s failed: %s\n", path, idev_afc_strerror(err));
    }

    return ret;
}


int get_afc_path(afc_client_t afc, const char *src, const char *dst)
{
    int ret=EXIT_FAILURE;

    if (idev_verbose)
        fprintf(stderr, "[debug] Downloading %s to %s - creating afc file connection\n", src, dst);

    uint64_t handle=0;
    afc_error_t err = afc_file_open(afc, src, AFC_FOPEN_RDONLY, &handle);

    if (err == AFC_E_SUCCESS) {
        char buf[CHUNKSZ];
        uint32_t bytes_read=0;
        size_t totbytes=0;

        FILE *outf = fopen(dst, "w");
        if (outf) {
            while((err=afc_file_read(afc, handle, buf, CHUNKSZ, &bytes_read)) == AFC_E_SUCCESS && bytes_read > 0) {
                totbytes += fwrite(buf, 1, bytes_read, outf);
            }
            fclose(outf);
            if (err) {
                fprintf(stderr, "Error: Encountered error while reading %s: %s\n", src, idev_afc_strerror(err));
                fprintf(stderr, "Warning! - %lu bytes read - incomplete data in %s may have resulted.\n", totbytes, dst);
            } else {
                printf("Saved %lu bytes to %s\n", totbytes, dst);
                ret=EXIT_SUCCESS;
            }

        } else {
            fprintf(stderr, "Error opening local file for writing: %s - %s\n", dst, strerror(errno));
        }

        afc_file_close(afc, handle);
    } else {
        fprintf(stderr, "Error: afc open file %s failed: %s\n", src, idev_afc_strerror(err));
    }

    return ret;
}

int put_afc_path(afc_client_t afc, const char *src, const char *dst)
{
    int ret=EXIT_FAILURE;

    uint64_t handle=0;

    FILE *inf = fopen(src, "r");
    if (inf) {
        if (idev_verbose)
            fprintf(stderr, "[debug] Uploading %s to %s - creating afc file connection\n", src, dst);

        afc_error_t err = afc_file_open(afc, dst, AFC_FOPEN_WRONLY, &handle);

        if (err == AFC_E_SUCCESS) {
            char buf[CHUNKSZ];
            uint32_t bytes_read=0;
            size_t totbytes=0;

            while(err==AFC_E_SUCCESS && (bytes_read=fread(buf, 1, CHUNKSZ, inf)) > 0) {
                uint32_t bytes_written=0;
                err=afc_file_write(afc, handle, buf, bytes_read, &bytes_written);
                totbytes += bytes_written;
            }

            if (err) {
                fprintf(stderr, "Error: Encountered error while writing %s: %s\n", src, idev_afc_strerror(err));
                fprintf(stderr, "Warning! - %lu bytes read - incomplete data in %s may have resulted.\n", totbytes, dst);
            } else {
                printf("Uploaded %lu bytes to %s\n", totbytes, dst);
                ret=EXIT_SUCCESS;
            }

            afc_file_close(afc, handle);
        } else {
            fprintf(stderr, "Error: afc open file %s failed: %s\n", src, idev_afc_strerror(err));
        }
        fclose(inf);
    } else {
        fprintf(stderr, "Error opening local file for reading: %s - %s\n", dst, strerror(errno));
    }

    return ret;
}


#pragma mark - Command handlers

int do_info(afc_client_t afc, int argc, char **argv)
{
    int j, i, ret = EXIT_SUCCESS;
    if (argc > 1) {
        for (i=1; i<argc ; i++) {
            ret |= dump_afc_file_info(afc, argv[i]);
        }
    } else {
        fprintf(stderr, "Error: you must specify at least one path.\n");
        ret = EXIT_FAILURE;
    }

    return ret;
}

int do_list(afc_client_t afc, int argc, char **argv)
{
    int i, ret = EXIT_SUCCESS;

    if (argc > 1) {
        for (i=1; i<argc ; i++) {
            ret |= dump_afc_list_path(afc, argv[i]);
        }
    } else {
        ret = dump_afc_list_path(afc, "");
    }

    return ret;
}


int do_mkdir(afc_client_t afc, int argc, char **argv)
{
    int i, ret=EXIT_SUCCESS;
    if (argc > 1) {
        for (i=1; i<argc ; i++) {
            afc_error_t err = afc_make_directory(afc, argv[i]);

            if (err == AFC_E_SUCCESS) {
                printf("Created directory: %s\n", argv[i]);
            } else {
                fprintf(stderr, "Error: mkdir error: %s\n", idev_afc_strerror(err));
                ret = EXIT_FAILURE;
            }
        }
    } else {
        fprintf(stderr, "Error: you must specify at least one directory path.\n");
        ret = EXIT_FAILURE;
    }

    return ret;
}

int do_rm(afc_client_t afc, int argc, char **argv)
{
    int i, ret=EXIT_SUCCESS;
    if (argc > 1) {
        for (i=1; i<argc ; i++) {
            afc_error_t err = afc_remove_path(afc, argv[i]);

            if (err == AFC_E_SUCCESS) {
                printf("Removed: %s\n", argv[i]);
            } else {
                fprintf(stderr, "Error: mkdir error: %s\n", idev_afc_strerror(err));
                ret = EXIT_FAILURE;
            }
        }
    } else {
        fprintf(stderr, "Error: you must specify at least one path to remove.\n");
        ret = EXIT_FAILURE;
    }

    return ret;
}

int do_rename(afc_client_t afc, int argc, char **argv)
{
    int ret = EXIT_FAILURE;

    if (argc == 3) {
        afc_error_t err = afc_rename_path(afc, argv[1], argv[2]);

        if (err == AFC_E_SUCCESS) {
            printf("Renamed %s to %s\n", argv[1], argv[2]);
            ret = EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Error: rename %s to %s - %s\n", argv[1], argv[2], idev_afc_strerror(err));
        }

    } else {
        fprintf(stderr, "Error: invalid number of arguments for rename.\n");
    }

    return ret;
}

int do_link(afc_client_t afc, int argc, char **argv)
{
    int ret=EXIT_FAILURE;

    if (argc == 3) {
        afc_error_t err = afc_make_link(afc, AFC_HARDLINK, argv[1], argv[2]);

        if (err == AFC_E_SUCCESS) {
            printf("Created hard-link %s -> %s\n", argv[2], argv[1]);
            ret = EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Error: link %s -> %s - %s\n", argv[2], argv[1], idev_afc_strerror(err));
        }

    } else {
        fprintf(stderr, "Error: invalid number of arguments for link command.\n");
    }

    return ret;
}

int do_symlink(afc_client_t afc, int argc, char **argv)
{
    int ret=EXIT_FAILURE;

    if (argc == 3) {
        afc_error_t err = afc_make_link(afc, AFC_SYMLINK, argv[1], argv[2]);

        if (err == AFC_E_SUCCESS) {
            printf("Created symbolic-link %s -> %s\n", argv[2], argv[1]);
            ret = EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Error: link %s -> %s - %s\n", argv[2], argv[1], idev_afc_strerror(err));
        }

    } else {
        fprintf(stderr, "Error: invalid number of arguments for link command.\n");
    }

    return ret;
}

int do_cat(afc_client_t afc, int argc, char **argv)
{
    int ret=EXIT_FAILURE;

    if (argc == 2) {
        ret = dump_afc_path(afc, argv[1], stdout);
    } else {
        fprintf(stderr, "Error: invalid number of arguments for cat command.\n");
    }

    return ret;
}

int do_get(afc_client_t afc, int argc, char **argv)
{
    int ret=EXIT_FAILURE;

    if (argc == 2) {
        ret = get_afc_path(afc, argv[1], basename(argv[1]));
    } else if (argc == 3) {
        char *dst = argv[2];
        char dpath[PATH_MAX];
        if (is_dir(dst)) {
            snprintf(dpath, PATH_MAX-1, "%s/%s", dst, basename(argv[1]));
            dst = dpath;
        }
        ret = get_afc_path(afc, argv[1], dpath);
    } else {
        fprintf(stderr, "Error: invalid number of arguments for get command.\n");
    }

    return ret;
}

int do_put(afc_client_t afc, int argc, char **argv)
{
    int ret=EXIT_FAILURE;

    if (argc == 2) {
        ret = put_afc_path(afc, argv[1], basename(argv[1]));
    } else if (argc == 3) {
        ret = put_afc_path(afc, argv[1], argv[2]);
    } else {
        fprintf(stderr, "Error: invalid number of arguments for put command.\n");
    }

    return ret;
}



int cmd_main(afc_client_t afc, int argc, char **argv)
{
        int ret=0;

        char *cmd = argv[0];

        if (!strcmp(cmd, "devinfo") || !strcmp(cmd, "deviceinfo")) {
            if (argc == 1) {
                ret = dump_afc_device_info(afc);
            } else {
                fprintf(stderr, "Error: unexpected extra arguments for devinfo\n");
                usage(stderr);
                ret=EXIT_FAILURE;
            }
        }
        else if (!strcmp(cmd, "info")) {
            ret = do_info(afc, argc, argv);
        }
        else if (!strcmp(cmd, "ls") || !strcmp(cmd, "list")) {
            ret = do_list(afc, argc, argv);
        }
        else if (!strcmp(cmd, "mkdir")) {
            ret = do_mkdir(afc, argc, argv);
        }
        else if (!strcmp(cmd, "rm") || !strcmp(cmd, "remove")) {
            ret = do_rm(afc, argc, argv);
        }
        else if (!strcmp(cmd, "rename")) {
            ret = do_rename(afc, argc, argv);
        }
        else if (!strcmp(cmd, "link") || !strcmp(cmd, "hardlink")) {
            ret = do_link(afc, argc, argv);
        }
        else if (!strcmp(cmd, "symlink")) {
            ret = do_symlink(afc, argc, argv);
        }
        else if (!strcmp(cmd, "cat")) {
            ret = do_cat(afc, argc, argv);
        }
        else if (!strcmp(cmd, "get")) {
            ret = do_get(afc, argc, argv);
        }
        else if (!strcmp(cmd, "put")) {
            ret = do_put(afc, argc, argv);
        }
        else {
            fprintf(stderr, "Error: unknown command: %s\n", cmd);
            usage(stderr);
            ret = EXIT_FAILURE;
        }

        return ret;
}

#define OPTION_FLAGS "rs:a:u:vh"
void usage(FILE *outf)
{
    fprintf(outf,
        "Usage: %s [%s] command cmdargs...\n\n"
        "  Options:\n"
        "    -r, --root                 Use the afc2 server if jailbroken (ignored with -a)\n"
        "    -s, --service=NAME>        Use the specified lockdown service (ignored with -a)\n"
        "    -a, --appid=<APP-ID>       Access bundle directory for app-id\n"
        "    -u, --uuid=<UDID>          Specify the device udid\n"
        "    -v, --verbose              Enable verbose debug messages\n"
        "    -h, --help                 Display this help message\n\n"

        "  Where \"command\" and \"cmdargs...\" are as folows:\n"
        "    devinfo                    dump device info from AFC server\n"
        "    list <dir> [dir2...]       list remote directory contents\n"
        "    info <path> [path2...]     dump remote file information\n"
        "    mkdir <path> [path2...]    create directory at path\n"
        "    rm <path> [path2...]       remove directory at path\n"
        "    rename <from> <to>         rename path 'from' to path 'to'\n"
        "    link <target> <link>       create a hard-link from 'link' to 'target'\n"
        "    symlink <target> <link>    create a symbolic-link from 'link' to 'target'\n"
        "    cat <path>                 cat contents of <path> to stdout\n"
        "    get <path> [localpath]     download a file (default: current dir)\n"
        "    put <localpath> [path]     upload a file (default: remote top-level dir)\n"
        , progname, OPTION_FLAGS);
}


static struct option longopts[] = {
    { "root",       no_argument,            NULL,   'r' },
    { "service",    required_argument,      NULL,   's' },
    { "appid",      required_argument,      NULL,   'a' },
    { "udid",       required_argument,      NULL,   'u' },
    { "verbose",    no_argument,            NULL,   'v' },
    { "help",       no_argument,            NULL,   'h' },
    { NULL,         0,                      NULL,   0 }
};

int main(int argc, char **argv)
{
    progname = basename(argv[0]);

    char *appid=NULL, *udid=NULL, *svcname=NULL;;

    svcname = AFC_SERVICE_NAME;

    int flag;
    while ((flag = getopt_long(argc, argv, OPTION_FLAGS, longopts, NULL)) != -1) {
        switch(flag) {
            case 'r':
                svcname = AFC2_SERVICE_NAME;
                break;

            case 's':
                svcname = optarg;
                break;

            case 'a':
                appid = optarg;
                break;

            case 'u':
                if ((strlen(optarg) != 40)) {
                    fprintf(stderr, "Error: invalid udid (wrong length): %s\n", optarg);
                    return EXIT_FAILURE;
                }

                udid = optarg;
                break;

            case 'v':
                idevice_set_debug_level(1);
                idev_verbose=true;
                break;

            case 'h':
                usage(stdout);
                return EXIT_SUCCESS;

            default:
                usage(stderr);
                return EXIT_FAILURE;

        }
    }

    argc -= optind;
    argv += optind;

    if (argc < 1) {
        fprintf(stderr, "Missing command argument\n");
        usage(stderr);
        return EXIT_FAILURE;
    }

    if (appid) {
        return idev_afc_app_client(progname, udid, appid, ^int(afc_client_t afc) {
            return cmd_main(afc, argc, argv);
        });
    } else {
        return idev_afc_client_ex(progname, udid, svcname, ^int(idevice_t idev, lockdownd_client_t client, lockdownd_service_descriptor_t ldsvc, afc_client_t afc) {
            return cmd_main(afc, argc, argv);
        });
    }
}

