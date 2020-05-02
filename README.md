# Nuke
Nuke is a program that can be used to 'nuke' a drive of its contents. It zeroes
out a given drive and writes random bytes for further data destruction.

```
Usage: nuke <drive 1> [drive 2]

Remove a drive(s) of its contents.

Options:
	-z, -0, --zero	Don't write random bytes to drive
	-h, --help	Display this help and exit
	-V, --version	Display this version information and exit

Example:
	nuke /dev/sdb
	nuke /dev/sdb /dev/sdc

NOTE: This program requires root privileges to run.
```

## Installation
Run ```make install``` to make and install nuke.
