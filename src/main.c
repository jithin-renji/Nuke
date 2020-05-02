/* main.c */

/* Nuke: A program to completely wipe drives of its contents, by various
 * methods of obfuscation. */

/*
 * 	Copyright (C) 2020 Jithin Renji
 *
 *	This file is part of Nuke.
 *
 *	Nuke is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Nuke is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Nuke.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <getopt.h>

#include "colors.h"
#include "nuke.h"

void usage (const char* progname);
void version (const char* progname);

struct option long_opt[] = {
	{"help", 	no_argument, 	0, 'h'},
	{"version", 	no_argument, 	0, 'V'},
	{0,		0,		0,  0}
};

int opt_index = 0;

int main (int argc, char** argv)
{
	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	} else {
		int opt = 0;

		while ((opt = getopt_long(argc, argv, "hV", long_opt, &opt_index)) != -1) {
			switch (opt) {
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			
			case 'V':
				version(argv[0]);
				exit(EXIT_SUCCESS);

			case '?':
				exit(EXIT_FAILURE);
			default:
				break;
			}
		}

		char** drvs = argv + optind;

		printf("List of drives to be nuked:\n");

		while (*drvs != NULL) {
			printf(B_GREEN "\t%s\n" RESET, *drvs);
			++drvs;
		}
		printf("\n");
		drvs = argv + optind;

		srand(time(NULL));
		while (*drvs != NULL) {
			int ret = nuke(*drvs);
			if (ret == -1) {
				exit(EXIT_FAILURE);
			}
			++drvs;
		}
	}

	return 0;
}

void usage (const char* progname)
{
	printf("Usage: %s <drive 1> [drive 2] ...\n\n", progname);

	printf("Remove a drive(s) of its contents.\n\n");

	printf("Options:\n"
	       "\t-h, --help\tDisplay this help and exit\n"
	       "\t-v, --version\tDisplay version information and exit\n\n"
	       "Examples:\n"
	       "\tnuke /dev/sdb\n"
	       "\tnuke /dev/sdb /dev/sdc\n\n");

	printf("NOTE: This program requires root privileges to run.\n");
}

void version (const char* progname)
{
printf("%s 0.01\n"
"Copyright (C) 2020 Jithin Renji.\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n\n"

"Written by Jithin Renji\n", progname);
}
