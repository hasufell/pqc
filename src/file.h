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
 * @file file.h
 * Header for the external API of file.c.
 * @brief header for file.c
 */

#ifndef NTRU_FILE_H
#define NTRU_FILE_H


#include "common.h"


/**
 * Reads a file and returns a newly allocated string.
 *
 * @param filename file to open
 * @return a newly allocated string which must be freed by the caller
 * or NULL on failure
 */
string *
read_file(char const * const filename);

void
write_file(string const *wstring, char const * const filename);


#endif /* NTRU_FILE_H */
