/*
 * Copyright (C) 2014 FH Bielefeld
 *
 * This file is part of a FH Bielefeld project.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

/**
 * @file ntru_file.c
 * Allows operations on files, such as reading
 * and writing.
 * @brief file operations
 */

#include "ntru_common.h"
#include "ntru_err.h"
#include "ntru_mem.h"
#include "ntru_string.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define STD_FILE_BUF 4096


/*------------------------------------------------------------------------*/

string *
read_file(char const * const filename)
{
	char buf[STD_FILE_BUF],
		 *cstring = NULL;
	int fd = 0;
	size_t str_size = 0;
	ssize_t n;
	string *result_string;

	fd = open(filename, O_RDONLY);

	if (fd != -1) {
		/* read and copy chunks */
		while ((n = read(fd, buf, STD_FILE_BUF)) != 0) {

			if (n == -1)
				NTRU_ABORT("Failed while reading file descriptor %d\n", fd);

			str_size += n; /* count total bytes read */

			REALLOC( /* allocate correct size */
					cstring, /* pointer to realloc */
					str_size /* total bytes read */
					+ 1); /* space for trailing NULL byte */

			/* append buffer to string */
			memcpy(cstring + (str_size - n), buf, (size_t)n);
		}
		/* add trailing NULL byte */
		cstring[str_size] = '\0';

		if (close(fd))
			NTRU_ABORT("Failed to close file descripter %d\n", fd);

		result_string = ntru_malloc(sizeof(*result_string));
		result_string->ptr = cstring;
		result_string->len = str_size;

		return result_string;

	} else {
		return NULL;
	}
}

/*------------------------------------------------------------------------*/

void
write_file(string const *wstring, char const * const filename)
{
	FILE *fp;

	fp = fopen(filename, "w");

	if (!fp)
		NTRU_ABORT("Failed while creating file\n");

	for (uint32_t i = 0; i < wstring->len; i++)
		fprintf(fp, "%c", wstring->ptr[i]);

	fclose(fp);
}

/*------------------------------------------------------------------------*/
