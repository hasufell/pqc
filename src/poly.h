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
 * @file poly.h
 * Header for the internal API of poly.c.
 * @brief header for poly.c
 */

#ifndef NTRU_POLY_H
#define NTRU_POLY_H

#include "context.h"
#include "err.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include <fmpz_poly.h>


void poly_new(fmpz_poly_t new_poly,
		int const * const c,
		const size_t len);

void poly_delete(fmpz_poly_t poly);

void poly_delete_all(fmpz_poly_t poly, ...);

void fmpz_poly_mod_unsigned(fmpz_poly_t a,
		unsigned int mod);

void fmpz_poly_mod(fmpz_poly_t a,
		unsigned int mod);

void fmpz_poly_set_coeff_fmpz_n(fmpz_poly_t poly,
		slong n,
		const fmpz_t x);

int fmpz_invmod_ui(fmpz_t f,
		const fmpz_t g,
		unsigned int mod);

void fmpz_add_n(fmpz_t f, const fmpz_t g, const fmpz_t h);

void poly_starmultiply(fmpz_poly_t a,
		fmpz_poly_t b,
		fmpz_poly_t c,
		ntru_context *ctx,
		unsigned int modulus);

bool poly_inverse_poly_q(fmpz_poly_t a,
		fmpz_poly_t Fq,
		ntru_context *ctx);

bool poly_inverse_poly_p(fmpz_poly_t a,
		fmpz_poly_t Fp,
		ntru_context *ctx);

void poly_draw(fmpz_poly_t poly);

void poly_draw_pretty(fmpz_poly_t poly);


#endif /* NTRU_POLY_H */
