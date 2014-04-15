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


#ifndef NTRU_POLY_H
#define NTRU_POLY_H

#include "context.h"

#include <tompoly.h>
#include <tommath.h>


void init_integer(mp_int *new_int);

void init_polynom(pb_poly *new_poly, mp_int *chara);

void init_polynom_size(pb_poly *new_poly, mp_int *chara, size_t size);

pb_poly *build_polynom(int const * const c,
		const size_t len,
		ntru_context *ctx);

void delete_polynom(pb_poly *new_poly);

void pb_starmultiply(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		ntru_context *ctx,
		unsigned int modulus);

void draw_polynom(pb_poly * const poly);

#endif /* NTRU_POLY_H */
