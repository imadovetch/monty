#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the stack_t structure
typedef struct stack_s {
    int n;
    struct stack_s *prev;
    struct stack_s *next;
} stack_t;

// Define the instruction_t structure
typedef struct instruction_s {
    char *opcode;
    void (*f)(stack_t **stack, unsigned int line_number);
} instruction_t;

// Declare global variable to store the push argument
extern int push_argument;

// Function prototypes
stack_t *addnode(stack_t **head, int data);
void printnodes(stack_t *head);
void pall(stack_t **stack, unsigned int line_number);
void push(stack_t **stack, unsigned int line_number);
void pop(stack_t **stack, unsigned int line_number);
void nop(stack_t **stack, unsigned int line_number);

#endif /* MAIN_H */
