# Nuke
Nuke is a program that can be used to 'nuke' a drive of its contents. At present, it just
zeroes out a given drive(s).

```
Usage: nuke <drive 1> [drive 2]

Remove a drive(s) of its contents.

Options:
	-h, --help	Display this help and exit
	-V, --version	Display this version information and exit

Example:
	nuke /dev/sdb
	nuke /dev/sdb /dev/sdc

NOTE: This program requires root privileges to run.
```
