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
 * @file ntru_cunit.c
 * Main cunit file, registering the test suites.
 * @brief test suite registration
 */

#include "ntru_cunit.h"

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

int main(void)
{
	CU_pSuite pSuite = NULL;
	int my_stderr;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("filereader tests",
		init_suite,
		clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test1 reading plain text file",
							 test_read_text_file1)) ||
		(NULL == CU_add_test(pSuite, "test2 reading plain text file",
							 test_read_text_file2)) ||
		(NULL == CU_add_test(pSuite, "test3 reading plain text file",
							 test_read_text_file3)) ||
		(NULL == CU_add_test(pSuite, "test4 reading plain text file",
							 test_read_text_file4)) ||
		(NULL == CU_add_test(pSuite, "test1 writing plain text file",
							 test_write_text_file1)) ||
		(NULL == CU_add_test(pSuite, "test2 writing plain text file",
							 test_write_text_file2)) ||
		(NULL == CU_add_test(pSuite, "test3 writing plain text file",
							 test_write_text_file3)) ||
		(NULL == CU_add_test(pSuite, "test4 writing plain text file",
							 test_write_text_file4)) ||
		(NULL == CU_add_test(pSuite, "test5 writing plain text file",
							 test_write_text_file5))
		) {

		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("polynomial tests",
		init_suite,
		clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test1 polynomial creation",
							 test_poly_new1)) ||
		(NULL == CU_add_test(pSuite, "test2 polynomial creation",
							 test_poly_new2))
		) {

		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("keypair tests",
		init_suite,
		clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test1 keypair creation",
							 test_create_keypair1)) ||
		(NULL == CU_add_test(pSuite, "test2 keypair creation",
							 test_create_keypair2)) ||
		(NULL == CU_add_test(pSuite, "test1 public key export",
							 test_export_public_key1)) ||
		(NULL == CU_add_test(pSuite, "test2 public key export",
							 test_export_public_key2)) ||
		(NULL == CU_add_test(pSuite, "test1 priv key export",
							 test_export_private_key1)) ||
		(NULL == CU_add_test(pSuite, "test2 priv key export",
							 test_export_private_key2)) ||
		(NULL == CU_add_test(pSuite, "test1 pub key import",
							 test_import_public_key1)) ||
		(NULL == CU_add_test(pSuite, "test2 pub key import",
							 test_import_public_key2)) ||
		(NULL == CU_add_test(pSuite, "test1 priv key import",
							 test_import_private_key1)) ||
		(NULL == CU_add_test(pSuite, "test2 priv key import",
							 test_import_private_key2))
		) {

		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("encryption tests",
		init_suite,
		clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test1 string encryption",
							 test_encrypt_string1))
		) {

		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("decryption tests",
		init_suite,
		clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test1 string decryption",
							 test_decrypt_string1))
		) {

		CU_cleanup_registry();
		return CU_get_error();
	}

	/* save stderr stream and close it */
	my_stderr = dup(STDERR_FILENO);
	close(STDERR_FILENO);

	/* Run all tests using the basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");

	/* Clean up registry and return */
	CU_cleanup_registry();

	/* restore stderr stream */
	dup2(my_stderr, STDERR_FILENO);

	return CU_get_error();
}
