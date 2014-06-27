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
 * @file ntru_encrypt_cunit.c
 * Test cases for encrypting strings.
 * @brief tests for ntru_encrypt.c
 */

#include "ntru.h"
#include "ntru_decrypt.h"
#include "ntru_keypair.h"

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * Test decrypting an encrypted string.
 */
void test_decrypt_string1(void)
{
	keypair pair;
	fmpz_poly_t f, g, rnd;
	int f_c[] = { -1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1 };
	int g_c[] = { -1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1 };
	int rnd_c[] = {-1, 0, 1, 1, 1, -1, 0, -1, 0, 0, 0};
	ntru_params params;
	string *enc_string,
		   *dec_string;
	char dec_c_str[1024];

	params.N = 11;
	params.p = 3;
	params.q = 32;

	poly_new(f, f_c, 11);
	poly_new(g, g_c, 11);
	poly_new(rnd, rnd_c, 11);

	ntru_create_keypair(&pair, f, g, &params);

	enc_string = read_file("to-decrypt.txt");
	dec_string = ntru_decrypt_string(enc_string, pair.priv, pair.priv_inv,
			&params);

	memcpy(dec_c_str, dec_string->ptr, dec_string->len);
	dec_c_str[dec_string->len] = '\0';
	string_delete(enc_string);
	string_delete(dec_string);

	CU_ASSERT_EQUAL(strcmp(dec_c_str, "BLAHFASEL\n"), 0);
}
