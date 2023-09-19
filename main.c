#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int push_argument = 0;

void pchar(stack_t **stack, unsigned int line_number);
void mul(stack_t **stack, unsigned int line_number);
void sub(stack_t **stack, unsigned int line_number);
void perform_division(stack_t **stack, unsigned int line_number);
void add(stack_t **stack, unsigned int line_number);
void print(stack_t **stack, unsigned int line_number);
void swap(stack_t **stack, unsigned int line_number);
void pstr(stack_t **stack, unsigned int line_number);
int main(int argc, char *argv[]) {
    instruction_t ops[] = {
        {"pall", pall},
        {"push", push},
        {"pop", pop},
        {"nop", nop},
        {"print", print},
        {"swap",swap},
        {"add",add},
        {"pstr",pstr}
    };
    stack_t *head = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char line[1024];
    

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
    if (push_argument == 0) {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    addnode(stack, push_argument);
}


void pop(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
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
        fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (*stack)->n);
}

void swap(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
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


void add(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }
    
    int sum = (*stack)->n + (*stack)->next->n;
    pop(stack, line_number);

    (*stack)->n = sum;
}

void sub(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't sub, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }
    
    int difference = (*stack)->next->n - (*stack)->n;
    pop(stack, line_number);

    (*stack)->n = difference;
}

void mul(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "Error: not enough elements in the stack to add\n");
        exit(EXIT_FAILURE);
    }
    
    int sum = (*stack)->n * (*stack)->next->n;
    pop(stack, line_number);

    (*stack)->n = sum;
}
void perform_division(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't div, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }
    
    if ((*stack)->n == 0) {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }

    int result = (*stack)->next->n / (*stack)->n;
    pop(stack, line_number);
    (*stack)->n = result;
}


void mod(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't mod, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }
    
    if ((*stack)->n == 0) {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }
    
    int result = (*stack)->next->n % (*stack)->n;
    pop(stack, line_number);

    (*stack)->n = result;
}

void pchar(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        fprintf(stderr, "L%u: can't pchar, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n < 0 || (*stack)->n > 127) {
        fprintf(stderr, "L%u: can't pchar, value out of range\n", line_number);
        exit(EXIT_FAILURE);
    }

    putchar((*stack)->n);
    putchar('\n');
    pop(stack, line_number);
}

void pstr(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        putchar('\n');
        return;
    }

    stack_t *current = *stack;
    while (current != NULL && current->n != 0 && (current->n >= 0 && current->n <= 127)) {
        putchar(current->n);
        current = current->next;
    }

    putchar('\n');
}

void rotl(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        return; // Nothing to rotate if the stack has less than two elements
    }

    stack_t *current = *stack;
    while (current->next != NULL) {
        current = current->next;
    }

    // Adjust the links to rotate the stack
    current->next = *stack;
    (*stack)->prev = current;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    current->next->next = NULL;
}

void rotr(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL || (*stack)->next == NULL) {
        // If the stack is empty or has only one element, rotr has no effect
        return;
    }

    stack_t *current = *stack;
    
    while (current->next != NULL) {
        current = current->next;
    }

    // Adjust the pointers to perform the rotation
    current->prev->next = NULL;
    current->prev = NULL;
    (*stack)->prev = current;
    current->next = *stack;
    current->prev = NULL;
    *stack = current;
}
