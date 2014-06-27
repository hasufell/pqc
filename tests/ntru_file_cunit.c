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
 * @file ntru_file_cunit.c
 * Test cases for file reading and writing.
 * @brief tests for ntru_file.c
 */

#include "ntru.h"

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * Read a valid text file and compare it with the correct expected
 * string.
 */
void test_read_text_file1(void)
{
	string *tmp_str = read_file("test-file.txt");
	char actual_string[512];
	char *expected_string = "This test file is a test file.\n";
	memcpy(actual_string, tmp_str->ptr, tmp_str->len);
	actual_string[tmp_str->len] = '\0';

	string_delete(tmp_str);

	CU_ASSERT_PTR_NOT_NULL(actual_string);
	CU_ASSERT_EQUAL((strcmp(actual_string, expected_string)), 0);
}

/**
 * Read a valid text file and compare it with an uncorrect expected
 * string.
 */
void test_read_text_file2(void)
{
	string *tmp_str = read_file("test-file.txt");
	char actual_string[512];
	char *expected_string = "foo\n";
	memcpy(actual_string, tmp_str->ptr, tmp_str->len);
	actual_string[tmp_str->len] = '\0';

	string_delete(tmp_str);

	CU_ASSERT_PTR_NOT_NULL(actual_string);
	CU_ASSERT_NOT_EQUAL((strcmp(actual_string, expected_string)), 0);
}

/**
 * Read an invalid text file.
 */
void test_read_text_file3(void)
{
	string *actual_string = read_file("asd");

	CU_ASSERT_PTR_NULL(actual_string);
}

/**
 * Pass NULL to the read_file() function.
 */
void test_read_text_file4(void)
{
	string *actual_string = read_file(NULL);

	CU_ASSERT_PTR_NULL(actual_string);
}

/**
 * Test if writing the file works, without checking the content.
 */
void test_write_text_file1(void)
{
	string *tmp_str = read_file("test-file.txt");
	write_file(tmp_str, "test-file.out");
	string_delete(tmp_str);

	CU_ASSERT_EQUAL(0, access("test-file.out", F_OK));
}

/**
 * Test if the written file has the expected content.
 */
void test_write_text_file2(void)
{
	string *tmp_str = read_file("test-file.txt");
	char actual_string[512];
	char *expected_string = "This test file is a test file.\n";

	write_file(tmp_str, "test-file.out");
	string_delete(tmp_str);
	tmp_str = read_file("test-file.out");

	memcpy(actual_string, tmp_str->ptr, tmp_str->len);
	actual_string[tmp_str->len] = '\0';

	string_delete(tmp_str);

	CU_ASSERT_PTR_NOT_NULL(actual_string);
	CU_ASSERT_EQUAL((strcmp(actual_string, expected_string)), 0);
}

/**
 * Check error handling when trying to write to directory.
 */
void test_write_text_file3(void)
{
	string str;
	char *c_str = "This test file is a test file.\n";
	str.ptr = c_str;
	str.len = 0;
	str.len = strlen(c_str);

	CU_ASSERT_EQUAL(false, write_file(&str, "."));
}

/**
 * Check error handling when passing NULL pointers.
 */
void test_write_text_file4(void)
{
	CU_ASSERT_EQUAL(false, write_file(NULL, "test-file.out"));
}

/**
 * Check error handling when passing NULL pointers.
 */
void test_write_text_file5(void)
{
	string str;
	char *c_str = "This test file is a test file.\n";
	str.ptr = c_str;
	str.len = strlen(c_str);

	CU_ASSERT_EQUAL(false, write_file(&str, NULL));
}
