#include "monty.h"

/**
 * addnode - Add a new node to the stack.
 * @head: A pointer to the stack.
 * @data: The data to add to the new node.
 * Return: A pointer to the new node.
 */
stack_t *addnode(stack_t **head, int data)
{
	stack_t *new = malloc(sizeof(stack_t));

	if (new == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	new->n = data;
	new->prev = NULL;
	new->next = *head;
	if (*head != NULL)
	{
		(*head)->prev = new;
	}
	*head = new;
	return (new);
}

/**
 * pall - Print all elements of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;
	(void)line_number;

	while (current != NULL)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}

/**
 * push - Push an element onto the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void push(stack_t **stack, unsigned int line_number)
{
	(void)line_number;
	addnode(stack, push_argument);
}

/**
 * pop - Pop the top element from the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void pop(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;

	if (*stack == NULL)
	{
		fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
		exit(EXIT_FAILURE);
	}
	*stack = current->next;
	if (current->next != NULL)
	{
		current->next->prev = NULL;
	}
	free(current);
}

/**
 * nop - Do nothing.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void nop(stack_t **stack, unsigned int line_number)
{
	(void)line_number;
	(void)stack;
}
