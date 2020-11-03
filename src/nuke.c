/* nuke.c: Core nuke functions */

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

int confirm (const char* drv, const int sects, const int pct);

//nsects option to erase the number of sectors and psects to erase percentage of disk sectors
int nuke (const char* drv, int only_zero, int nreps, int nsects, float pct, int ask_confirm)

{


	int fd_drv = open(drv, O_RDWR);  //Mode read write

	/* Stats */
	struct stat drv_stat; //Structure that contains information regarding the file.

	/*   #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>
	*/


	unsigned long nblocks_drv = 0;
	long long bytes_drv = 0;

	/* Optimal block size */
	size_t bs = 0;

	if (fd_drv == -1) {
		perror(drv); //Print drv name to stderr if file doesnt exist.
		return -1;
	}

	int r_drv = ioctl(fd_drv, BLKGETSIZE, &nblocks_drv);
	//#include <sys/ioctl.h> : Get me the size in sectors (number of sectors) of fd_drv (BLKGETSIZE) to be stored on nblocks_drv. zero success, otherwise -1 (few exceptions)

	//BLKGETSIZE: device depend request code, this one tells specifically to ioctl to get the size of a block device

	if (r_drv == -1) {
		perror(drv);
		return -1;
	}

	fstat(fd_drv, &drv_stat);
	//https://linux.die.net/man/2/fstat get stats of a file, actually it returns a stats structure, in this case will be passed to drv_stat

	bs = drv_stat.st_blksize; //Block size for filesystem I/O

  //bs = 4096



  if(nsects){
		bytes_drv = 512 * nsects;
	}else if(pct){
		bytes_drv = 512 * (nblocks_drv * (pct/100));
	}else{
		bytes_drv = 512 * nblocks_drv; //Size of disk sector is of 512 bytes.
	}


	int cnfrm = 1;

	if (ask_confirm == 1) {
		cnfrm = confirm(drv, nsects, pct);
	}

	if (cnfrm) {
		/* Location to seek to */
		off_t seek_loc = 0;
		for (int i = 0; i < nreps; i++) {
			if (nreps != 1) {
				printf(B_CYAN "STAGE %d:\n" RESET, i + 1);
			}
			clear_drv (fd_drv, bytes_drv, bs, seek_loc);

			if (!only_zero) {

				 /*Why random bytes and then clear??*/

				rand_drv (fd_drv, bytes_drv, bs, seek_loc);
				clear_drv (fd_drv, bytes_drv, bs, seek_loc);
			}
		}
	} else {
		printf("Aborted.\n");
	}

	return 0;
}

int confirm (const char* drv, const int sects, const int pct)
{


	if(sects){

			printf(B_RED "WARNING: " WHITE "Contents from the first to the %dth sectors on '%s' "
			B_RED "CANNOT BE RECOVERED " WHITE "after this operation\n\
			and all data on this range will be " B_RED "PERMENANTLY DELETED!\n\n" RESET, sects, drv);

			printf("Do you " B_WHITE "STILL" WHITE " want to continue? [" B_RED "yes" WHITE "/" B_GREEN "NO" RESET "] ");


 	}else if(pct){

		printf(B_RED "WARNING: " WHITE "%d%% of disk sector's contents on '%s' "
		B_RED "CANNOT BE RECOVERED " WHITE "after this operation\n\
		and all data on this range will be " B_RED "PERMENANTLY DELETED!\n\n" RESET, pct, drv);

		printf("Do you " B_WHITE "STILL" WHITE " want to continue? [" B_RED "yes" WHITE "/" B_GREEN "NO" RESET "] ");


	}else{


			printf(B_RED "WARNING: " WHITE "The contents of '%s' "
			B_RED "CANNOT BE RECOVERED " WHITE "after this operation\n\
			and all data on this device will be " B_RED "PERMENANTLY DELETED!\n\n" RESET, drv);

			printf("Do you " B_WHITE "STILL" WHITE " want to continue? [" B_RED "yes" WHITE "/" B_GREEN "NO" RESET "] ");





 }


	char response[512];
	fgets(response, 512, stdin);


	if (strcmp(response, "yes\n") == 0) {
		return 1;
	}

	return 0;
}

void clear_drv (int fd_drv, size_t count, size_t bs, off_t seek_loc)//off_t size is 8 bytes in
{
	lseek(fd_drv, seek_loc, SEEK_SET); //moves file offset to 0, start of the file.
	/*
		Number of bytes written, is
		used to calculate the percentage of
		progress
	*/
	long double nbytes_written = 0;

	/* Buffer to store 0's to be written */
	char buf[bs];

	memset(buf, 0, sizeof(buf)); //Writes 0's bytes to buf

	while (nbytes_written != count && nbytes_written < count) {
		int ret = write(fd_drv, buf, bs);

		if (ret == -1) {
			perror("");
			exit(EXIT_FAILURE);
		}

		long double percent = (nbytes_written/count) * 100;

		/* Hide cursor */
		fputs("\e[?25l", stdout);

		printf("\tClearing %ld byte(s). [%.2Lf%%]\r", count, percent);
		fflush(stdout);

		/* Show cursor */
		fputs("\e[?25h", stdout);

		nbytes_written += bs;
	}
	printf("\n");
}

//Why using seek_loc if the offset will be zero anyway?

void rand_drv (int fd_drv, size_t count, size_t bs, off_t seek_loc)
{
	lseek(fd_drv, seek_loc, SEEK_SET);
	/* Same as clear_drv() */
	long double nbytes_written = 0;

	/* Buffer to store random bytes */
	char buf[bs];

	while (nbytes_written != count && nbytes_written < count) {
		for (int i = 0; i < bs; i++) {
			buf[i] = rand() % 256;
		}

		int ret = write(fd_drv, buf, bs);

		if (ret == -1) {
			perror("");
			exit(EXIT_FAILURE);
		}

		long double percent = (nbytes_written/count) * 100;

		fputs("\e[?25l", stdout);

		printf("\tWriting %ld random byte(s). [%.2Lf%%]\r", count, percent);
		fflush(stdout);

		fputs("\e[?25h", stdout);

		nbytes_written += bs;
	}
	printf("\n");
}
