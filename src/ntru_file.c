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
	size_t file_length = 0;
	string *result_string;
	struct stat s;

	if (!filename)
		return NULL;

	fd = open(filename, O_RDONLY);

	/* check if this is a real file */
	if (fstat(fd, &s) == -1)
		return NULL;
	if (!S_ISREG(s.st_mode))
		return NULL;

	file_length = lseek(fd, 0, SEEK_END) + 1;
	lseek(fd, 0, SEEK_SET);

	cstring = malloc(sizeof(char) * file_length);

	if (fd != -1) {
		/* read and copy chunks */
		while ((n = read(fd, buf, STD_FILE_BUF)) != 0) {

			if (n == -1) {
				NTRU_WARN_DEBUG("Failed while reading file descriptor %d",
						fd);
				goto failure_cleanup;
			}

			str_size += n; /* count total bytes read */

			/* append buffer to string */
			memcpy(cstring + (str_size - n), buf, (size_t)n);
		}
		/* add trailing NULL byte */
		cstring[str_size] = '\0';

		if (close(fd)) {
			NTRU_WARN_DEBUG("Failed to close file descripter %d\n", fd);
			goto failure_cleanup;
		}

		result_string = ntru_malloc(sizeof(*result_string));
		result_string->ptr = cstring;
		result_string->len = str_size;

		return result_string;

	} else {
		return NULL;
	}

failure_cleanup:
	free(cstring);
	return NULL;
}

/*------------------------------------------------------------------------*/

bool
write_file(string const *wstring, char const * const filename)
{
	FILE *fp;
	struct stat s;

	if (!wstring || !filename)
		return false;

	/* if "filename" already exists, we need to make sure
	 * it's a regular file */
	if (stat(filename, &s) == 0 &&
			!S_ISREG(s.st_mode))
		return false;

	fp = fopen(filename, "w");

	if (!fp) {
		NTRU_WARN_DEBUG("Failed while creating file\n");
		return false;
	}

	for (uint32_t i = 0; i < wstring->len; i++)
		fprintf(fp, "%c", wstring->ptr[i]);

	if (fclose(fp)) {
		NTRU_WARN_DEBUG("Failed to close file descripter\n");
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------*/
