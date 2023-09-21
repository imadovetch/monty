#include "monty.h"

/**
 * sub - Subtract the top element from the second top element of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void sub(stack_t **stack, unsigned int line_number)
{
	int difference;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't sub, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}
	difference = (*stack)->next->n - (*stack)->n;
	pop(stack, line_number);
	(*stack)->n = difference;
}

/**
 * mul - Multiply the top two elements of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void mul(stack_t **stack, unsigned int line_number)
{
	int product;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "Error: not enough elements in the stack to add\n");
		exit(EXIT_FAILURE);
	}
	product = (*stack)->n * (*stack)->next->n;
	pop(stack, line_number);
	(*stack)->n = product;
}

/**
 * perform_division - Divide the second
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void perform_division(stack_t **stack, unsigned int line_number)
{
	int result;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't div, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}
	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%u: division by zero\n", line_number);
		exit(EXIT_FAILURE);
	}
	result = (*stack)->next->n / (*stack)->n;
	pop(stack, line_number);
	(*stack)->n = result;
}

/**
 * mod - Compute the modulo of the second top
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void mod(stack_t **stack, unsigned int line_number)
{
	int result;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't mod, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%u: division by zero\n", line_number);
		exit(EXIT_FAILURE);
	}

	result = (*stack)->next->n % (*stack)->n;
	pop(stack, line_number);

	(*stack)->n = result;
}
