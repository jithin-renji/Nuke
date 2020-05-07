# Nuke
Nuke is a program that can be used to 'nuke' a drive of its contents. It zeroes
out a given drive and writes random bytes for further data destruction.

NOTE: At the moment, Nuke can only be run on Linux.
```
Usage: nuke <drive 1> [drive 2]

Remove a drive(s) of its contents.

Options:
	-z, -0, --zero	Don't write random bytes to drive
	-n, --num-reps	Number of times to repeat the process (defaults to 1)
	-h, --help	Display this help and exit
	-V, --version	Display this version information and exit

Examples:
	nuke /dev/sdb
	nuke /dev/sdb /dev/sdc
	nuke -z /dev/sdb
	nuke -n 2 /dev/sdb

NOTE: This program requires root privileges to run.
```

## Installation
Run ```make install``` to make and install nuke.
