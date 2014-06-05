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
 * @file rnd.c
 * This file allows generation of random polynomials.
 * @brief random polynomials
 */

#include "err.h"
#include "math.h"
#include "params.h"
#include "poly.h"

#include <fmpz_poly.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


/*------------------------------------------------------------------------*/

int
get_rnd_int(void)
{
	int rnd_data = open("/dev/random", O_RDONLY);
	int rnd_int;
	size_t rnd_len = 0;

	while (rnd_len < sizeof(rnd_int)) {
		ssize_t result = read(rnd_data, ((char*)&rnd_int) + rnd_len,
				sizeof(rnd_int) - rnd_len);

		if (result < 0)
			NTRU_ABORT("Unable to read /dev/random!\n");

		rnd_len += result;
	}

	close(rnd_data);

	return rnd_int;
}

/*------------------------------------------------------------------------*/

int
get_urnd_int(void)
{
	int rnd_data = open("/dev/urandom", O_RDONLY);
	int rnd_int;
	ssize_t result;

	result = read(rnd_data, ((char*)&rnd_int),
			sizeof(rnd_int));

	if (result < 0)
		NTRU_ABORT("Unable to read /dev/urandom!\n");

	close(rnd_data);

	return rnd_int;
}

/*------------------------------------------------------------------------*/

void
ntru_get_rnd_tern_poly_num(fmpz_poly_t poly,
		const ntru_params *params,
		uint32_t num_ones,
		uint32_t num_neg_ones,
		int (*rnd_int)(void))
{
	if (!poly || ! params)
		NTRU_ABORT("unexpected NULL parameters in"
				"ntru_get_rnd_tern_poly_num()!\n");

	fmpz_poly_zero(poly);

	while (num_ones != 0 || num_neg_ones != 0) {
		int32_t pos = rnd_int() % params->N;

		if (!fmpz_cmp_si_n(fmpz_poly_get_coeff_ptr(poly, pos), 0)) {
			if (num_ones > 0) {
				fmpz_poly_set_coeff_si(poly, pos, 1);
				num_ones--;
			} else if (num_neg_ones > 0) {
				fmpz_poly_set_coeff_si(poly, pos, -1);
				num_neg_ones--;
			}
		}
	}
}

/*------------------------------------------------------------------------*/
