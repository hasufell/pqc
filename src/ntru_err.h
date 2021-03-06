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
 * @file ntru_err.h
 * This file provides error macros
 * and functions for the NTRU cryptosystem.
 * @brief error handling
 */

#ifndef NTRU_ERR_H
#define NTRU_ERR_H


/**
 * Print a message to stderr and abort the program.
 */
#define NTRU_ABORT(...) \
{ \
	fprintf(stderr, __VA_ARGS__); \
	abort(); \
}

/**
 * Print a message to stderr, giving the
 * current file and line and abort the program
 * and finally abort the program.
 */
#define NTRU_ABORT_DEBUG(...) \
{ \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, " [file %s, line %d]:\n", \
				__FILE__, __LINE__); \
	abort(); \
}

/**
 * Print a message to stderr.
 */
#define NTRU_WARN(...) \
{ \
	fprintf(stderr, __VA_ARGS__); \
}

/**
 * Print a message to stderr, giving the
 * current file and line.
 */
#define NTRU_WARN_DEBUG(...) \
{ \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, " [file %s, line %d]:\n", \
				__FILE__, __LINE__); \
}


#endif /* NTRU_ERR_H */
