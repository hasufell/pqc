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
 * The maximal integer that is given by
 * ntru_get_urnd_poly_big and ntru_get_rnd_poly_big
 */
#define BIG_RAND_MAX 100

/**
 * The minimal integer that is given by
 * ntru_get_urnd_poly_big and ntru_get_rnd_poly_big
 */
#define BIG_RAND_MIN -100

pb_poly *ntru_get_urnd_poly_small(ntru_context *ctx);
pb_poly *ntru_get_rnd_poly_small(ntru_context *ctx);
pb_poly *ntru_get_urnd_poly_big(ntru_context *ctx);
pb_poly *ntru_get_rnd_poly_big(ntru_context *ctx);

#endif /* NTRU_RAND_H */
