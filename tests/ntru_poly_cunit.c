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
 * @file ntru_poly_cunit.c
 * Test cases for polynomial creation.
 * @brief tests for ntru_poly.c
 */

#include "ntru.h"

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fmpz_poly.h>
#include <fmpz.h>


/**
 * Test creating a new polynomial.
 */
void test_poly_new1(void)
{
	fmpz_poly_t new_poly;
	int c[11] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, -1 , 1};

	poly_new(new_poly, c, 11);

	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 0), 1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 1), 0), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 2), 1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 3), 0), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 4), 1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 5), 1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 6), 0), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 7), 1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 8), 0), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 9), -1), 0);
	CU_ASSERT_EQUAL(fmpz_cmp_si(fmpz_poly_get_coeff_ptr(new_poly, 10), 1), 0);
}

/**
 * Test creating a new polynomial, without coefficients.
 */
void test_poly_new2(void)
{
	fmpz_poly_t new_poly;

	poly_new(new_poly, NULL, 0);

	CU_ASSERT_PTR_NULL(fmpz_poly_get_coeff_ptr(new_poly, 0));
}
