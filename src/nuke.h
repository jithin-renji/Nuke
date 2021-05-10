/* nuke.h: This is the main header */

/*
 *    Copyright (C) 2020 Jithin Renji
 *
 *    This file is part of Nuke.
 *
 *    Nuke is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Nuke is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Nuke.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUKE_H
#define NUKE_H

#include <sys/types.h>

int nuke(const char* drv, int only_zero, int nreps, int ask_confirm);
int confirm(const char* drv);
void clear_drv(int fd_drv, size_t count, size_t bs, off_t seek_loc);
void rand_drv(int fd_drv, size_t count, size_t bs, off_t seek_loc);

#endif    /* NUKE_H */
