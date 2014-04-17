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

#include "context.h"
#include "err.h"
#include "rand.h"
#include "poly.h"

#include <stdio.h>
#include <stdlib.h>
#include <tompoly.h>
#include <tommath.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * static declarations
 */
static mp_digit get_urnd_int_small(int *sign);
static mp_digit get_rnd_int_small(int *sign);

/**
 * Gets randomly a small integer
 * from the set {-1, 0, 1} using /dev/random.
 *
 * @param sign stores the signness [out]
 * @return random small integer
 */
static mp_digit get_rnd_int_small(int *sign)
{
	int random_data;
	mp_digit random_int;
	size_t randomDataLen = 0;
	random_data = open("/dev/random", O_RDONLY);

	while (randomDataLen < sizeof(random_int)) {
		ssize_t result = read(random_data,
				((char*) &random_int) + randomDataLen,
				(sizeof(random_int)) - randomDataLen);
		if (result < 0) {
			NTRU_ABORT("Unable to read /dev/random");
		}
		randomDataLen += result;
	}
	close(random_data);

	random_int = random_int % 3;

	if (random_int == 1) {
		*sign = 0;
	} else if (random_int == 2) {
		random_int = 1;
		*sign = 1;
	} else {
		*sign = 0;
	}
	return random_int;
}

/**
 * Gets randomly a small integer
 * from the set {-1, 0, 1} using /dev/urandom.
 *
 * @param sign stores the signness [out]
 * @return random small integer
 */
static mp_digit get_urnd_int_small(int *sign)
{
	int random_data;
	mp_digit random_int;
	ssize_t result;

	random_data = open("/dev/urandom", O_RDONLY);
	if ((result = read(random_data, &random_int, sizeof(random_int))) < 0)
		NTRU_ABORT("Unable to read /dev/urandom");
	close(random_data);

	random_int = random_int % 3;

	if (random_int == 1) {
		*sign = 0;
	} else if (random_int == 2) {
		random_int = 1;
		*sign = 1;
	} else {
		*sign = 0;
	}

	return random_int;
}

/**
 * Gets a random polynomial with coefficients
 * from the set {-1 ,0 ,1} using /dev/urandom.
 *
 * @param ctx the NTRU context
 * @return newly allocated polynomial, must be freed with delete_polynom()
 */
pb_poly *ntru_get_urnd_poly_small(ntru_context *ctx)
{
	mp_int chara;
	init_integer(&chara);
	pb_poly *poly = malloc(sizeof(pb_poly));
	init_polynom_size(poly, &chara, ctx->N);
	mp_clear(&chara);

	for (int i = 0; i < ctx->N; i++) {
		int sign;
		int c = get_urnd_int_small(&sign);

		mp_set(&(poly->terms[i]), (mp_digit) c);

		if (sign == 1)
			poly->terms[i].sign = 1;
	}
	poly->used = ctx->N;
	pb_clamp(poly);

	return poly;
}

