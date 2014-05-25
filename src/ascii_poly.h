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

#ifndef ASCII_POLY_H_
#define ASCII_POLY_H_


#include "context.h"

#include <fmpz_poly.h>
#include <fmpz.h>


fmpz_poly_t **ascii_to_poly(char *to_poly, ntru_context *ctx);
char *poly_to_ascii(fmpz_poly_t **poly_array, ntru_context *ctx);


#endif /* ASCII_POLY_H_ */
