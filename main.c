#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int push_argument = 0;

/**
 * main - Entry point for the Monty interpreter.
 * @argc: The number of command-line arguments.
 * @argv: An array of command-line argument strings.
 * Return: EXIT_SUCCESS if successful, EXIT_FAILURE on failure.
 */
int main(int argc, char *argv[])
{
	char line[1024];
	FILE *file;
	int found;

	instruction_t ops[] = {
		{"pall", pall},
		{"push", push},
		{"pop", pop},
		{"pint", print},
		{"swap", swap},
		{"add", add},
		{"mul", mul},
		{"sub", sub},
		{"pstr", pstr},
		{"pchar", pchar},
		{"nop", nop},
		{"rotr", rotr},
		{"rotl", rotl},
		{"div", perform_division},
		{"mod", mod},
	};

	stack_t *head = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	file = fopen(argv[1], "r");
	while (fgets(line, sizeof(line), file))
	{
		long unsigned int in;
		char *c_ommand[2];

		c_ommand[0] = strtok(line, " \n\t");
		c_ommand[1] = strtok(NULL, " \n\t");
		if (c_ommand[0] == NULL || c_ommand[0][0] == '#')
			continue;
		found = 0;
		for (in = 0; in < sizeof(ops) / sizeof(ops[0]); in++)
		{
			if (strcmp(c_ommand[0], ops[in].opcode) == 0)
			{
				if (strcmp(c_ommand[0], "push") == 0)
				{
					if (c_ommand[1] == NULL || !is_valid_integer(c_ommand[1]))
					{
						fprintf(stderr, "L%lu: usage: push integer\n", in + 1);
						fclose(file);
						while (head != NULL)
						{
							stack_t *temp = head;
							head = head->next;
							free(temp);
						}
						return (EXIT_FAILURE);
					}
					push_argument = atoi(c_ommand[1]);
				}
				ops[in].f(&head, 1);
				found = 1;
				break;
			}
		}
		if (!found)
			fprintf(stderr, "Unknown opcode: %s", line);
	}
	fclose(file);
	while (head != NULL)
	{
		stack_t *temp = head;
		head = head->next;
		free(temp);
	}
	return (EXIT_SUCCESS);
}

/**
 * is_valid_integer - Check if a string is a valid integer.
 * @str: The input string.
 * Return: 1 if valid, 0 if not.
 */
int is_valid_integer(const char *str)
{
	size_t i;

	if (str == NULL || *str == '\0')
	{
		return (0);
	}
	for (i = 0; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i]) && !(i == 0 && str[i] == '0' && str[i + 1] == '\0'))
		{
			return (0);
		}
	}
	return (1);
}

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
 * add - Add the top two elements of the stack.
 * @stack: A pointer to the stack.
 * @line_number: The current line number.
 */
void add(stack_t **stack, unsigned int line_number)
{
	int sum;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}
	sum = (*stack)->n + (*stack)->next->n;
	pop(stack, line_number);

	(*stack)->n = sum;
}

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
 * perform_division - Divide the second top element by the top element of the stack.
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
 * mod - Compute the modulo of the second top element by the top element of the stack.
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
