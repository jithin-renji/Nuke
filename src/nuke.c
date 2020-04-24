/* nuke.c: Core nuke functions */

/*
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
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#include <unistd.h>
#include <fcntl.h>

#include "nuke.h"
#include "colors.h"

int confirm (const char* drv);

int nuke (const char* drv)
{
	int fd_drv = open(drv, O_RDWR);

	/* Stats */	
	struct stat drv_stat;
	unsigned long nblocks_drv = 0;
	long long bytes_drv = 0;

	/* Optimal block size */
	size_t bs = 0;

	if (fd_drv == -1) {
		perror(drv);
		return -1;
	}

	int r_drv = ioctl(fd_drv, BLKGETSIZE, &nblocks_drv);

	if (r_drv == -1) {
		perror(drv);
		return -1;
	}
	
	fstat(fd_drv, &drv_stat);
	bs = drv_stat.st_blksize;
	bytes_drv = bs * nblocks_drv;

	int cnfrm = confirm(drv);

	if (cnfrm) {
		clear_drv (fd_drv, bytes_drv, bs);
	} else {
		printf("Aborted.\n");
	}

	return 0;
}

int confirm (const char* drv)
{
	printf(B_RED "WARNING: " WHITE "The contents of '%s' "
	       B_RED "CANNOT BE RECOVERED " WHITE "after this operation\n\
         and all data on this device will be " B_RED "PERMENANTLY DELETED!\n\n" RESET, drv);

	printf("Do you " B_WHITE "STILL" WHITE " want to continue? [" B_RED "yes" WHITE "/" B_GREEN "NO" RESET "] ");

	char response[512];
	fgets(response, 512, stdin);

	if (strcmp(response, "yes\n") == 0) {
		return 1;
	}

	return 0;
}

void clear_drv (int fd_drv, size_t count, size_t bs)
{
	/*
		Number of bytes written, is
		used to calculate the percentage of
		progress
	*/
	long double nbytes_written = 0;

	/* Buffer to store 0's to be written */
	char buf[512];

	memset(buf, 0, sizeof(buf));

	while (nbytes_written != count) {
		int ret = write(fd_drv, buf, 512);

		if (ret == -1) {
			perror("");
			exit(3);
		}
		
		long double percent = (nbytes_written/count) * 100;

		/* Hide cursor */
		fputs("\e[?25l", stdout);

		printf("Clearing %ld byte(s). [%.2Lf%%]\r", count, percent);
		fflush(stdout);

		/* Show cursor */
		fputs("\e[?25h", stdout);

		nbytes_written += bs;
	}
	printf("\n");
}
