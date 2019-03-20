////////////////////////////////////////////////////////////////////////////////
//
//  File          : asgn3.c
//  Description   : This is the main code file for the CMSC257 assignment 3.
//                  -----description-------
//
//  Author        : Mark Groves
//  Created       : 03/19/19
//


//Project Includes
#include "asgn3_support.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
// #include <stdlib.h>       use for testing with ACTUAL malloc()

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : This is the main function for the asgn3.c program.
//
// Inputs       : none
// Outputs      : 0 if successful, -1 otherwise

int main( void )
{
    int *block1 = malloc(82);
    printf("%d", *block1);
    // Return successfully
    return(0);
}