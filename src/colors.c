/* colors.c: Useful functions for color output */

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

#include "colors.h"

void set_color (const char* color)
{
	printf("%s", color);
}

void reset (void)
{
	printf(RESET);
}

int cprintf (const char* format, const char* color, ...)
{
	va_list args;
	va_start(args, color);

	set_color(color);
	int r = vprintf(format, args);
	reset();

	va_end(args);

	return r;
}
