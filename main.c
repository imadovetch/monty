#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monty.h"

int push_argument = 0;

void cleanup(stack_t **head, FILE *file) {
    fclose(file);
    while (*head != NULL) {
        stack_t *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void process_file(FILE *file, stack_t **head, instruction_t ops[], size_t num_ops) {
    char line[1024];
    int found, count = 0;

    while (fgets(line, sizeof(line), file)) {
        long unsigned int in;
        char *c_ommand[2];
        count++;
        c_ommand[0] = strtok(line, " \n\t");
        c_ommand[1] = strtok(NULL, " \n\t");
        
        if (c_ommand[0] == NULL || c_ommand[0][0] == '#')
            continue;
        
        found = 0;
        for (in = 0; in < num_ops; in++) {
            if (strcmp(c_ommand[0], ops[in].opcode) == 0) {
                if (strcmp(c_ommand[0], "push") == 0) {
                    if (c_ommand[1] == NULL || !is_valid_integer(c_ommand[1])) {
                        fprintf(stderr, "L%d: usage: push integer\n", count);
                        cleanup(head, file);
                        exit(EXIT_FAILURE);
                    }
                    push_argument = atoi(c_ommand[1]);
                }
                ops[in].f(head, count);
                found = 1;
                break;
            }
        }

        if (!found) {
            fprintf(stderr, "L%d: unknown instruction %s\n", count, c_ommand[0]);
            cleanup(head, file);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *file = NULL;

    instruction_t ops[] = {
        {"pall", pall}, {"push", push}, {"pop", pop},
        {"pint", print}, {"swap", swap},
        {"add", add}, {"mul", mul},
        {"sub", sub}, {"pstr", pstr},
        {"pchar", pchar}, {"nop", nop}, {"rotr", rotr},
        {"rotl", rotl}, {"div", perform_division}, {"mod", mod},
    };

    size_t num_ops = sizeof(ops) / sizeof(ops[0]);

    stack_t *head = NULL;

    if (argc != 2) {
        fprintf(stderr, "USAGE: monty file\n");
        return (EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error: Can't open file");
        fprintf(stderr, "%s\n", argv[1]);
        return (EXIT_FAILURE);
    }

    process_file(file, &head, ops, num_ops);
    
    fclose(file);
    cleanup(&head, file);
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
