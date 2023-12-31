#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * struct stack_s - Doubly linked list representation of a stack (or queue)
 * @n: Integer stored in the node
 * @prev: Pointer to the previous element of the stack (or queue)
 * @next: Pointer to the next element of the stack (or queue)
 */
typedef struct stack_s
{
int n;
struct stack_s *prev;
struct stack_s *next;
} stack_t;

/**
 * struct instruction_s - Opcode and its associated function
 * @opcode: The opcode
 * @f: The function to handle the opcode
 */
typedef struct instruction_s
{
char *opcode;
void (*f)(stack_t **stack, unsigned int line_number);
} instruction_t;

extern int push_argument;

stack_t *addnode(stack_t **head, int data);
void pall(stack_t **stack, unsigned int line_number);
void push(stack_t **stack, unsigned int line_number);
void pop(stack_t **stack, unsigned int line_number);
void nop(stack_t **stack, unsigned int line_number);
void pchar(stack_t **stack, unsigned int line_number);
void mul(stack_t **stack, unsigned int line_number);
void sub(stack_t **stack, unsigned int line_number);
void perform_division(stack_t **stack, unsigned int line_number);
void add(stack_t **stack, unsigned int line_number);
void print(stack_t **stack, unsigned int line_number);
void swap(stack_t **stack, unsigned int line_number);
void pstr(stack_t **stack, unsigned int line_number);
int is_valid_integer(const char *str);
void rotr(stack_t **stack, unsigned int line_number);
void rotl(stack_t **stack, unsigned int line_number);
void mod(stack_t **stack, unsigned int line_number);

#endif /* MAIN_H */
