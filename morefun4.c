#include "monty.h"

/**
 * swap - Swap the top two elements of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void swap(stack_t **stack, unsigned int line_number)
{
	stack_t *tmp1 = *stack;
	stack_t *tmp2 = (*stack)->next;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}
	tmp1->next = tmp2->next;
	tmp2->prev = NULL;
	tmp2->next = tmp1;
	tmp1->prev = tmp2;
	*stack = tmp2;
}
/**
 * print - Print the top element of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void print(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL)
	{
		fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
		exit(EXIT_FAILURE);
	}
	printf("%d\n", (*stack)->n);
}
