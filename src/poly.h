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
#include <stdarg.h>
#include <stdbool.h>


unsigned int get_degree(pb_poly const * const poly);

#define MP_SET(...) mp_set(__VA_ARGS__)

#define MP_SET_INT(...) \
{ \
	int result; \
	if ((result = mp_set_int(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error setting long constant. %s", \
				mp_error_to_string(result)); \
}

#define MP_MUL(...) \
{ \
	int result; \
	if ((result = mp_mul(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error multiplying terms. %s", \
				mp_error_to_string(result)); \
}

#define MP_DIV(...) \
{ \
	int result; \
	if ((result = mp_div(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error dividing terms. %s", \
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

#define MP_EXPTMOD(...) \
{ \
	int result; \
	if ((result = mp_exptmod(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error computing modular exponentiation. %s", \
				mp_error_to_string(result)); \
}

#define MP_INVMOD(...) \
{ \
	int result; \
	if ((result = mp_invmod(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error computing modular inverse. %s", \
				mp_error_to_string(result)); \
}

#define MP_EXPT_D(...) \
{ \
	int result; \
	if ((result = mp_expt_d(__VA_ARGS__)) != MP_OKAY) \
			NTRU_ABORT("Error computing modular exponentiation. %s", \
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

#define PB_MOD(poly_a, mp_int, poly_out, len) \
{ \
	for (unsigned int i = 0; i < len; i++) \
		MP_DIV(&(poly_a->terms[i]), mp_int, NULL, &(poly_out->terms[i])); \
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

void delete_polynom_multi(pb_poly *poly, ...);

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

bool pb_inverse_poly_p(pb_poly *a,
		pb_poly *Fp,
		ntru_context *ctx);

void draw_polynom(pb_poly * const poly);

#endif /* NTRU_POLY_H */
