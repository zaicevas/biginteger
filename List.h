/* Singly linked list, modified for usage alongside BigInteger
AUTHOR: Tomas Zaicevas, Valdas Rakutis
Software Engineering, Vilnius University, 1st year
2018 Spring */
#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum {false, true} bool;

typedef enum {
    ALL_GOOD = 0,
    OUT_OF_MEMORY_EXCEPTION = -1,
    LIST_EMPTY_EXCEPTION = -2,
    INDEX_OUT_OF_RANGE_EXCEPTION = -3,
    DIVISION_BY_ZERO_EXCEPTION = -4
} ErrorCode;

typedef struct Node {
    char data;
    struct Node *next;
} Node;

typedef struct List {
    Node *HEAD;
    unsigned count;
} List;

/* ADD / REMOVE NODE */
ErrorCode push_front(List*, char);                    /* push entry at the front of list */
ErrorCode push_back(List*, char);                   /* push entry at the end of list */
char pop_front(List*);                              /* returns first value of the list */
char pop_back(List*);                               /* returns last value of the list */
ErrorCode insert(List*, unsigned, char);            /* insert entry by index [1, n] */
ErrorCode delete_by_index(List*, unsigned);         /* delete entry by index [1, n] */

/* PARSE */
List* string_to_list(char*);                        /* returns the specified string in a list of chars
                                                     returns list with a single zero, if parsed string is not a number*/
char* list_to_string(List*, char);                    /* converts a given List to a c string */
void print_list(List);

/* List manipulations */
List* create_list(unsigned int size);                /* creates a list of a specified size (can be zero)
                                                    and returns the list of that size with all values
                                                    initialized to 0 */
List* list_cpy(List*);                                /* copies a given list to a new List (does not delete the old one) */
List* reverse_list(List *list);
Node* get_by_index(List*, unsigned index);            /* returns the pointer to the specified Node */
void free_list(List*);                              /* free memory and dissolve the list */
void check_error(ErrorCode code);                    /* prints error according the the error code */
bool is_empty(List);

/* LOGICAL OPERATIONS */
List* get_longer(List *a, List *b, int *length);    /* returns the longer list of the two and it's length */
List* get_shorter(List *a, List *b, int *length);    /* returns the shorter list of the two and it's length */
#endif
