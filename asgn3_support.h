#include <stddef.h>
#ifndef A3SUPPORT_INCLUDED
#define A3SUPPORT_INCLUDED


//////////////////////////////////////////////////////////////////////////////
//
//  File          : asgn3_support.h
//  Description   : This is the header file for the functions for assignment
//                  3 of the CMSC257 course. This file defines
//                  the following functions that are implemented in another
//                  file, asgn3.c
//
//  Author        : Mark Groves
//  Created       : 03/19/19

// Functions to define

void *malloc(size_t size);
void *calloc(size_t nitems, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif