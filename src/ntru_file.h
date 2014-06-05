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
 * @file ntru_file.h
 * Header for the external API of ntru_file.c.
 * @brief header for ntru_file.c
 */

#ifndef NTRU_FILE_H
#define NTRU_FILE_H


#include "ntru_common.h"
#include "ntru_string.h"

#include <stdbool.h>


/**
 * Reads a file and returns a newly allocated string.
 *
 * @param filename file to open
 * @return a newly allocated string which must be freed by the caller
 * or NULL on failure (e.g. if the file could not be opened/closed)
 */
string *
read_file(char const * const filename);

/**
 * Write a string to a file. The file will be pruned or created
 * if it does not exist.
 *
 * @param wstring the string to write to the file
 * @param filename the name of the file to write to
 * @return true for success or false for failure if fopen or fclose failed
 */
bool
write_file(string const *wstring, char const * const filename);


#endif /* NTRU_FILE_H */
