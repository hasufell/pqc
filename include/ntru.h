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
 * @file ntru.h
 * This file holds the public API of the most basic
 * data types and operations of the pqc NTRU implementation
 * and is meant to be installed on the client system.
 * @brief public API, basic data types
 */

#ifndef PUBLIC_NTRU_NTRU_H_
#define PUBLIC_NTRU_NTRU_H_


#include <fmpz_poly.h>
#include <fmpz.h>
#include <stdint.h>


typedef struct ntru_params ntru_params;
typedef struct string string;


/**
 * NTRU cryptosystem is specified by
 * the following triple.
 */
struct ntru_params {
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
};

/**
 * Represents a string.
 */
struct string {
	/**
	 * Pointer to the char array,
	 * holding the actual string.
	 * THIS IS NOT NULL TERMINATED
	 * (at least not necessarily,
	 * don't ever assume it).
	 */
	char *ptr;
	/**
	 * Length of the string.
	 */
	size_t len;
};


/**
 * Prints the given string to stdout.
 *
 * @param print_string the print to string
 */
void
prints(const string *print_string);

/**
 * Delete the inner structure
 * of the string and frees the string
 * itself from the heap. Must not be
 * called on stack variables.
 *
 * @param del_string the string to delete
 */
void
string_delete(string *del_string);

/**
 * Initializes and builds a polynomial with the
 * coefficient values of c[] of size len within NTRU
 * parameters and returns a newly allocated polynomial.
 * For an empty polynom, both c and len can be NULL/0.
 *
 * @param new_poly the polynomial to initialize and
 * fill with coefficients [out]
 * @param c array of polynomial coefficients, can be NULL
 * @param len size of the coefficient array, can be 0
 * @return newly allocated polynomial pointer, must be freed
 * with fmpz_poly_clear()
 */
void
poly_new(fmpz_poly_t new_poly,
		int const * const c,
		const size_t len);

/**
 * This deletes the internal structure of a polynomial,
 * and frees the pointer.
 *
 * @param poly the polynomial to delete
 */
void
poly_delete(fmpz_poly_t poly);

/**
 * Delete the internal structure of a polynomial
 * array which must be NULL terminated. It is expected
 * that poly_array is not on the stack and was obtained
 * by a function like ascii_to_poly().
 *
 * @param poly_array the polynomial array
 */
void
poly_delete_array(fmpz_poly_t **poly_array);

/**
 * This deletes the internal structure of all polynomials,
 * and frees the pointers.
 * You must call this with NULL as last argument!
 *
 * @param poly the polynomial to delete
 * @param ... follow up polynomials
 */
void
poly_delete_all(fmpz_poly_t poly, ...);

/**
 * Draws a polynomial to stdout.
 *
 * @param poly draw this
 */
void
poly_draw(const fmpz_poly_t poly);

/**
 * Draws a polynomial to stdout,
 * in pretty format.
 *
 * @param poly draw this
 */
void
poly_draw_pretty(const fmpz_poly_t poly);

/**
 * Reads a file and returns a newly allocated string.
 *
 * @param filename file to open
 * @return a newly allocated string which must be freed by the caller
 * or NULL on failure
 */
string *
read_file(char const * const filename);

/**
 * Write a string to a file. The file will be pruned or created
 * if it does not exist.
 *
 * @param wstring the string to write to the file
 * @param filename the name of the file to write to
 */
void
write_file(string const *wstring, char const * const filename);


#endif /* NTRU_NTRU_H_ */
