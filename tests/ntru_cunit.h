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
 * @file ntru_cunit.h
 * @brief header for ntru_cunit.c
 */

/*
 * filereader/writer
 */
void test_read_text_file1(void);
void test_read_text_file2(void);
void test_read_text_file3(void);
void test_read_text_file4(void);
void test_write_text_file1(void);
void test_write_text_file2(void);
void test_write_text_file3(void);
void test_write_text_file4(void);
void test_write_text_file5(void);

/*
 * poly
 */
void test_poly_new1(void);
void test_poly_new2(void);

/*
 * keypair
 */
void test_create_keypair1(void);
void test_create_keypair2(void);
void test_export_public_key1(void);
void test_export_public_key2(void);
void test_export_private_key1(void);
void test_export_private_key2(void);
void test_import_public_key1(void);
void test_import_public_key2(void);
void test_import_private_key1(void);
void test_import_private_key2(void);

/*
 * encryption
 */
void test_encrypt_string1(void);

/*
 * decryption
 */
void test_decrypt_string1(void);
