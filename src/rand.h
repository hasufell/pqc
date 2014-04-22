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

#ifndef NTRU_RAND_H
#define NTRU_RAND_H

#include "context.h"

#include <tompoly.h>

/**
 * Use the /dev/urandom device as entropy source.
 */
#define GET_INT_FROM_URAND 2

/**
 * Use the /dev/random device as entropy source.
 */
#define GET_INT_FROM_RRAND 3

pb_poly *ntru_get_poly_small(int length, int entropy_source);
pb_poly *ntru_get_poly_big(int length, int entropy_source, mp_int *upper_bound,
		mp_int *lower_bound);

#endif /* NTRU_RAND_H */
