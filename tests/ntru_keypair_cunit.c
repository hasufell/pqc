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
 * @file ntru_keypair_cunit.c
 * Test cases for generating private and public keys.
 * @brief tests for ntru_keypair.c
 */

#include "ntru.h"
#include "ntru_keypair.h"

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * Test keypair creation.
 */
void test_create_keypair1(void)
{
	keypair pair;
	fmpz_poly_t f, g, pub, priv_inv;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	int pub_c[] = { 8, 25, 22, 20, 12, 24, 15, 19, 12, 19, 16 };
	int priv_inv_c[] = { 1, 2, 0, 2, 2, 1, 0, 2, 1, 2, 0 };
	ntru_params params;
	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);
	poly_new(pub, pub_c, 11);
	poly_new(priv_inv, priv_inv_c, 11);

	CU_ASSERT_EQUAL(true, ntru_create_keypair(&pair, f, g, &params));
	CU_ASSERT_EQUAL(1, fmpz_poly_equal(pub, pair.pub));
	CU_ASSERT_EQUAL(1, fmpz_poly_equal(priv_inv, pair.priv_inv));
	CU_ASSERT_EQUAL(1, fmpz_poly_equal(f, pair.priv));
}

/**
 * Test keypair creation error handling via non-invertible polynomial.
 */
void test_create_keypair2(void)
{
	keypair pair;
	fmpz_poly_t f, g, priv_inv;
	int f_c[] = { 0, 0, 1, 0, -1, 0, 0, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	int priv_inv_c[] = { 1, 2, 0, 2, 2, 1, 0, 2, 1, 2, 0 };
	ntru_params params;
	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);
	poly_new(priv_inv, priv_inv_c, 11);

	CU_ASSERT_EQUAL(false, ntru_create_keypair(&pair, f, g, &params));
}

/**
 * Test exporting public key and reading the resulting file.
 */
void test_export_public_key1(void)
{
	keypair pair;
	fmpz_poly_t f, g;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;
	string *pub_string;
	char *expected_pub_c_str = "CBkWFAwYDxMMExA=";
	char actual_pub_c_str[512] = "\0";

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_public_key("pub.key", pair.pub, &params);

	if ((pub_string = read_file("pub.key"))) {
		memcpy(actual_pub_c_str, pub_string->ptr, pub_string->len);
		actual_pub_c_str[pub_string->len] = '\0';
		string_delete(pub_string);
	}

	remove("pub.key");

	CU_ASSERT_EQUAL(strcmp(expected_pub_c_str, actual_pub_c_str), 0);
}

/**
 * Test error handling of exporting public key.
 */
void test_export_public_key2(void)
{
	keypair pair;
	fmpz_poly_t f, g;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;
	string *pub_string;
	char *expected_pub_c_str = "CBkWFAwYDxMMExA=";
	char actual_pub_c_str[512] = "\0";

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_public_key(".", pair.pub, &params);

	if ((pub_string = read_file("pub.key"))) {
		memcpy(actual_pub_c_str, pub_string->ptr, pub_string->len);
		actual_pub_c_str[pub_string->len] = '\0';
		string_delete(pub_string);
	}

	CU_ASSERT_NOT_EQUAL(strcmp(expected_pub_c_str, actual_pub_c_str), 0);
}

/**
 * Test exporting private key and reading the resulting file.
 */
void test_export_private_key1(void)
{
	keypair pair;
	fmpz_poly_t f, g;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;
	string *pub_string;
	char *expected_priv_c_str = "AgEBAgAAAAEAAQE=";
	char actual_priv_c_str[512] = "\0";

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_priv_key("priv.key", pair.pub, &params);

	if ((pub_string = read_file("priv.key"))) {
		memcpy(actual_priv_c_str, pub_string->ptr, pub_string->len);
		actual_priv_c_str[pub_string->len] = '\0';
		string_delete(pub_string);
	}

	remove("priv.key");

	CU_ASSERT_EQUAL(strcmp(expected_priv_c_str, actual_priv_c_str), 0);
}

/**
 * Test error handling of exporting private key.
 */
void test_export_private_key2(void)
{
	keypair pair;
	fmpz_poly_t f, g;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;
	string *pub_string;
	char *expected_priv_c_str = "AgEBAgAAAAEAAQE=";
	char actual_priv_c_str[512] = "\0";

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_priv_key(".", pair.pub, &params);

	if ((pub_string = read_file("priv.key"))) {
		memcpy(actual_priv_c_str, pub_string->ptr, pub_string->len);
		actual_priv_c_str[pub_string->len] = '\0';
		string_delete(pub_string);
	}

	CU_ASSERT_NOT_EQUAL(strcmp(expected_priv_c_str, actual_priv_c_str), 0);
}

/**
 * Test importing public key and reading.
 */
void test_import_public_key1(void)
{
	keypair pair;
	fmpz_poly_t f, g, pub;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;

	fmpz_poly_init(pub);

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_public_key("pub.key", pair.pub, &params);
	import_public_key(pub, "pub.key", &params);

	remove("pub.key");

	CU_ASSERT_EQUAL(1, fmpz_poly_equal(pub, pair.pub));
}

/**
 * Test error handling of importing public key.
 */
void test_import_public_key2(void)
{
	keypair pair;
	fmpz_poly_t f, g, pub;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;

	fmpz_poly_init(pub);

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_public_key("pub.key", pair.pub, &params);
	import_public_key(pub, "foo", &params);

	remove("pub.key");

	CU_ASSERT_NOT_EQUAL(1, fmpz_poly_equal(pub, pair.pub));
}

/**
 * Test importing private key and reading.
 */
void test_import_private_key1(void)
{
	keypair pair;
	fmpz_poly_t f, g, priv, priv_inv;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;

	fmpz_poly_init(priv);
	fmpz_poly_init(priv_inv);

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_priv_key("priv.key", pair.priv, &params);
	import_priv_key(priv, priv_inv, "priv.key", &params);

	remove("priv.key");

	CU_ASSERT_EQUAL(1, fmpz_poly_equal(priv, pair.priv));
}

/**
 * Test error handling of importing private key.
 */
void test_import_private_key2(void)
{
	keypair pair;
	fmpz_poly_t f, g, priv, priv_inv;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	ntru_params params;

	fmpz_poly_init(priv);
	fmpz_poly_init(priv_inv);

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);

	ntru_create_keypair(&pair, f, g, &params);
	export_priv_key("priv.key", pair.priv, &params);
	import_priv_key(priv, priv_inv, ".", &params);

	remove("priv.key");

	CU_ASSERT_NOT_EQUAL(1, fmpz_poly_equal(priv, pair.priv));
}
