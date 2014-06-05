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
 * @file params.h
 * This file defines the ntru_params
 * and related data types.
 * @brief NTRU parameters
 */

#ifndef NTRU_PARAMS_H
#define NTRU_PARAMS_H


#include <stdint.h>


/**
 * NTRU cryptosystem is specified by
 * the following triple.
 */
typedef struct {
	/**
	 * maximal degree N - 1 for
	 * all polynomials
	 */
	uint32_t N;
	/**
	 * large modulus
	 */
	uint32_t q;
	/**
	 * small modulus
	 */
	uint32_t p;
} ntru_params;

#endif /* NTRU_PARAMS_H */
