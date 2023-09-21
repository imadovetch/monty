#include "main.h"

/**
 * pchar - Print the character at the top of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void pchar(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL)
	{
		fprintf(stderr, "L%u: can't pchar, stack empty\n", line_number);
		exit(EXIT_FAILURE);
	}

	if ((*stack)->n < 0 || (*stack)->n > 127)
	{
		fprintf(stderr, "L%u: can't pchar, value out of range\n", line_number);
		exit(EXIT_FAILURE);
	}

	putchar((*stack)->n);
	putchar('\n');
	pop(stack, line_number);
}

/**
 * pstr - Print the string starting from the top of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void pstr(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;

	(void)line_number;
	if (*stack == NULL)
	{
		putchar('\n');
		return;
	}

	while (current != NULL && current->n != 0 &&
	(current->n >= 0 && current->n <= 127))
	{
		printf("%c", current->n);
		current = current->next;
	}

	putchar('\n');
}

/**
 * rotl - Rotate the stack to the left.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void rotl(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;

	(void)line_number;
	if (*stack == NULL || (*stack)->next == NULL)
	{
		return;
	}

	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = *stack;
	(*stack)->prev = current;
	*stack = (*stack)->next;
	(*stack)->prev = NULL;
	current->next->next = NULL;
}

/**
 * rotr - Rotate the stack to the right.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void rotr(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;
	(void)line_number;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		return;
	}

	while (current->next != NULL)
	{
		current = current->next;
	}
	current->prev->next = NULL;
	current->prev = NULL;
	(*stack)->prev = current;
	current->next = *stack;
	current->prev = NULL;
	*stack = current;
}
