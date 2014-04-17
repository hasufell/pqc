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
#include "err.h"

#include <tompoly.h>
#include <tommath.h>
#include <stdbool.h>

#define MP_MUL(...) \
{ \
	int result; \
	if ((result = mp_mul(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error multiplying terms. %s", \
				mp_error_to_string(result)); \
}

#define MP_ADD(...) \
{ \
	int result; \
	if ((result = mp_add(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error adding terms. %s", \
				mp_error_to_string(result)); \
}

#define MP_SUB(...) \
{ \
	int result; \
	if ((result = mp_sub(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error substracting terms. %s", \
				mp_error_to_string(result)); \
}

#define MP_MOD(...) \
{ \
	int result; \
	if ((result = mp_mod(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error reducing term by modulo. %s", \
				mp_error_to_string(result)); \
}

#define MP_COPY(...) \
{ \
	int result; \
	if ((result = mp_copy(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error copying terms. %s", \
				mp_error_to_string(result)); \
}

#define MP_XOR(...) \
{ \
	int result; \
	if ((result = mp_xor(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error XORing terms. %s", \
				mp_error_to_string(result)); \
}

#define PB_MUL(...) \
{ \
	int result; \
	if ((result = pb_mul(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error multiplying polynomials. %s", \
				mp_error_to_string(result)); \
}

#define PB_ADD(...) \
{ \
	int result; \
	if ((result = pb_add(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error adding polynomials. %s", \
				mp_error_to_string(result)); \
}

#define PB_SUB(...) \
{ \
	int result; \
	if ((result = pb_sub(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error substracting polynomials. %s", \
				mp_error_to_string(result)); \
}

#define PB_MOD(poly_a, mp_int, poly_out, context) \
{ \
	for (unsigned int i = 0; i < context->N; i++) \
		MP_MOD(&(poly_a->terms[i]), mp_int, &(poly_out->terms[i])); \
}

#define PB_COPY(...) \
{ \
	int result; \
	if ((result = pb_copy(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error copying polynomial. %s", \
				mp_error_to_string(result)); \
}

void init_integer(mp_int *new_int);

void init_polynom(pb_poly *new_poly, mp_int *chara);

void init_polynom_size(pb_poly *new_poly, mp_int *chara, size_t size);

pb_poly *build_polynom(int const * const c,
		const size_t len,
		ntru_context *ctx);

void erase_polynom(pb_poly *poly, size_t len);

void delete_polynom(pb_poly *new_poly);

void pb_starmultiply(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		ntru_context *ctx,
		unsigned int modulus);

void pb_xor(pb_poly *a,
		pb_poly *b,
		pb_poly *c,
		const size_t len);

bool pb_inverse_poly_q(pb_poly *a,
		pb_poly *Fq,
		ntru_context *ctx);

void draw_polynom(pb_poly * const poly);

#endif /* NTRU_POLY_H */
