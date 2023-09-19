#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int push_argument = 0; // Define and initialize push_argument

// Function prototypes
void print(stack_t **stack, unsigned int line_number);
void swap(stack_t **stack, unsigned int line_number);
int main(int argc, char *argv[]) {
    instruction_t ops[] = {
        {"pall", pall},
        {"push", push},
        {"pop", pop},
        {"nop", nop},
        {"print", print},
        {"swap",swap}
    };
    stack_t *head = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char line[1024];
    int count = 0;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        char command[20];
        if (sscanf(line, "%19s", command) == 1) {
            int found = 0; // Flag to indicate if the opcode is found
            for (int in = 0; in < sizeof(ops) / sizeof(ops[0]); in++) {
                if (strcmp(command, ops[in].opcode) == 0) {
                    if (strcmp(command, "push") == 0) {
                        if (sscanf(line, "%*s %d", &push_argument) != 1) {
                            fprintf(stderr, "Invalid push format: %s", line);
                            break;
                        }
                    }
                    ops[in].f(&head, 1);
                    found = 1; // Set the flag to indicate opcode found
                    break; // Break the loop once the opcode is found
                }
            }
            if (!found) {
                fprintf(stderr, "Unknown opcode: %s", line);
            }
        } else if (strlen(line) > 1) {
            fprintf(stderr, "Invalid line format: %s", line);
        }

        count++;
        if (count > 10) {
            fprintf(stderr, "Warning: Maximum number of commands reached (10).\n");
            break;
        }
    }

    fclose(file);

    // Cleanup: Free the remaining nodes in the stack
    while (head != NULL) {
        stack_t *temp = head;
        head = head->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

stack_t *addnode(stack_t **head, int data) {
    stack_t *new = malloc(sizeof(stack_t));

    if (new == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    new->n = data;
    new->prev = NULL;
    new->next = *head;
    
    if (*head != NULL) {
        (*head)->prev = new;
    }
    
    *head = new;
    return new;
}

void pall(stack_t **stack, unsigned int line_number) {
    stack_t *current = *stack;
    while (current != NULL) {
        printf("%d\n", current->n);
        current = current->next;
    }
}

void push(stack_t **stack, unsigned int line_number) {
    addnode(stack, push_argument);
}

void pop(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        fprintf(stderr, "Error: can't pop an empty stack\n");
        exit(EXIT_FAILURE);
    }

    stack_t *current = *stack;

    *stack = current->next;

    if (current->next != NULL) {
        current->next->prev = NULL;
    }

    free(current);
}

void nop(stack_t **stack, unsigned int line_number) {
    (void)line_number; // Unused parameter
    (void)stack; // Unused parameter
    // Do nothing
}

void print(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        fprintf(stderr, "Error: can't print, stack is empty\n");
        exit(EXIT_FAILURE);
    }

    
    
    printf("%d\n", (*stack)->n);
}
void swap(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "Error: can't swap, stack has less than two elements\n");
        exit(EXIT_FAILURE);
    }

    stack_t *tmp1 = *stack;
    stack_t *tmp2 = (*stack)->next;

    tmp1->next = tmp2->next;
    tmp2->prev = NULL;

    tmp2->next = tmp1;
    tmp1->prev = tmp2;

    *stack = tmp2;
}

