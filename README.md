<img src="img/Nuke.png" alt="Nuke Logo" width=200 height=200/>

# Nuke

Nuke is a program that can be used to destroy the contents of a given
drive. It zeroes out a given drive and writes random bytes for further
data destruction.

```
Usage: nuke <drive 1> [drive 2] ...

Destroy the contents of a drive/drives.

Options:
    -z, -0, --zero    Don't write random bytes to drive
    -n, --repeat      Number of times to repeat the process (defaults to 1)
    -Y                Don't ask for confirmation (NOT RECOMMENDED)
    -h, --help        Display this help and exit
    -V, --version     Display this version information and exit

Examples:
    nuke /dev/sdb
    nuke /dev/sdb /dev/sdc
    nuke -z /dev/sdb
    nuke -n 2 /dev/sdb

NOTE: This program requires root privileges to run.
```

## Requirements

1. GCC
2. GNU Make
3. PyQt5 (for the GUI front-end)

## Build

NOTE: At the moment, Nuke can only be installed and run on Linux.

* Download the source tree and change to the source directory.

* Run `make` if you only want the command-line utility.

* To compile the front-end, run `make ui`.

## Install

Run `sudo make install` to make and install Nuke.

## Uninstall

Run `sudo make uninstall` to uninstall Nuke.
