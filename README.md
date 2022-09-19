afcclient
=========

A simple CLI interface to AFC via libimobiledevice.


## Requirements

- libimobiledevice (v 1.1.5+)
  https://github.com/libimobiledevice/libimobiledevice

- For building, clang is also required (support for Blocks).

- On Linux you will need the BlocksRuntime development libs. On Ubuntu the package is called 'libblocksruntime-dev'.

## Building

    $ make

## Usage

    Usage: afcclient [ra:u:vh] command cmdargs...

      Options:
        -r, --root                 Use the afc2 server if jailbroken (ignored with -a)
        -a, --appid=<APP-ID>       Access doc dir for app-id (paths still require Documents/ prefix)
        -u, --uuid=<UDID>          Specify the device udid
        -v, --verbose              Enable verbose debug messages
        -h, --help                 Display this help message

      Where "command" and "cmdargs..." are as folows:
        devinfo                    dump device info from AFC server
        ls <dir> [dir2...]         list remote directory contents
        info <path> [path2...]     dump remote file information
        mkdir <path> [path2...]    create directory at path
        rm <path> [path2...]       remove directory at path
        rename <from> <to>         rename path 'from' to path 'to'
        link <target> <link>       create a hard-link from 'link' to 'target'
        symlink <target> <link>    create a symbolic-link from 'link' to 'target'
        cat <path>                 cat contents of <path> to stdout
        get <path> [localpath]     download a file (default: current dir)
        put <localpath> [path]     upload a file (default: remote top-level dir)


## Known Issues / TODO

- listing output is fugly

## Author

Eric Monti - esmonti at gmail dot com


## License

MIT - See LICENSE.txt
