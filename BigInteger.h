/* BigInteger implementation in C, using singly-linked list
AUTHOR: Tomas Zaicevas, Valdas Rakutis
Software Engineering, Vilnius University, 1st year
2018 Spring */

#ifndef BIGINTEGER_H
#define BIGINTEGER_H
#include "List.h"

#define bint BigInteger
#define mtable MultiplicationTable

typedef enum {positive = 0, negative = 1} Sign;

typedef struct BigInteger {
    List *list;
    Sign sign;
} BigInteger;

typedef struct MultiplicationTable {
    bint values[10];
} MultiplicationTable;

/* parse functions */
bint string_to_bint(char*);            /* converts a string to a big integer */
bint int_to_bint(int);                /* converts an integer to a big integer */
char* bint_to_string(bint);            /* converts a big integer to an ascii string */
void print_bint(bint);

/* ALGEBRAIC OPERATIONS: */
bint bint_add(bint, bint);            /* a + b */
bint bint_sub(bint, bint);            /* a - b */
bint bint_mul(bint, bint);            /* a * b */
bint bint_div(bint, bint);            /* a / b */
bint bint_mod(bint, bint);            /* a b */

/* logical operations */
bool bint_equal(bint a, bint b);                        /* true if a == b */
bool bint_greater_than(bint a, bint b);                 /* true if a > b */
bool bint_lesser_than(bint a, bint b);                  /* true if a < b */
bool is_zero(bint);                         /* checks and returs 1 if the given bint is zero
                                            else returns 0*/

/* memory management */
bint bint_cpy(bint);                /*copies a given integer to a new memory location, does not delete the old one*/
void free_bint(bint);               /*free allocated memory for big integer*/
#endif
