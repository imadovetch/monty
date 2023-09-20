#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int push_argument = 0;

int main(int argc, char *argv[]) {
    char line[1024];
    FILE *file;int found;

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
        {"nop", nop}
    };
    stack_t *head = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {

        long unsigned int in;
        char *c_ommand[2];
        char command[20] ="";

        c_ommand[0] = strtok(line, " \n\t");
        c_ommand[1] = strtok(NULL, " \n\t");
        if (c_ommand[0] == NULL || command[0] == '#')
            continue;

        found = 0;

        for (in = 0; in < sizeof(ops) / sizeof(ops[0]); in++) {
            if (strcmp(c_ommand[0], ops[in].opcode) == 0) {
                if (strcmp(c_ommand[0], "push") == 0) {
                if (c_ommand[1] == NULL || !is_valid_integer(c_ommand[1])) {
                    fprintf(stderr, "L%lu: usage: push integer\n", in + 1);
                    fclose(file);
                    while (head != NULL) {
                        stack_t *temp = head;
                        head = head->next;
                        free(temp);
                    }
                    return EXIT_FAILURE;
                }
                push_argument = atoi(c_ommand[1]);
            }
                ops[in].f(&head, 1);
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Unknown opcode: %s", line);
        }
    }

    fclose(file);

    while (head != NULL) {
        stack_t *temp = head;
        head = head->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}
int is_valid_integer(const char *str) {
    size_t i;

    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && !(i == 0 && str[i] == '0' && str[i + 1] == '\0')) {
            return 0;
        }
    }

    return 1;
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
    (void)line_number;


    while (current != NULL) {
        printf("%d\n", current->n);
        current = current->next;
    }
}


void push(stack_t **stack, unsigned int line_number) {
    (void)line_number;

    addnode(stack, push_argument);
}


void pop(stack_t **stack, unsigned int line_number) {
    stack_t *current = *stack;

    if (*stack == NULL) {
        fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
        exit(EXIT_FAILURE);
    }


    *stack = current->next;

    if (current->next != NULL) {
        current->next->prev = NULL;
    }

    free(current);
}

void nop(stack_t **stack, unsigned int line_number) {
    (void)line_number;
    (void)stack;
}

void print(stack_t **stack, unsigned int line_number) {
    if (*stack == NULL) {
        fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (*stack)->n);
}

void swap(stack_t **stack, unsigned int line_number) {
    stack_t *tmp1 = *stack;
    stack_t *tmp2 = (*stack)->next;

    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }




    tmp1->next = tmp2->next;
    tmp2->prev = NULL;

    tmp2->next = tmp1;
    tmp1->prev = tmp2;

    *stack = tmp2;
}


void add(stack_t **stack, unsigned int line_number) {
    int sum;

    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    sum = (*stack)->n + (*stack)->next->n;
    pop(stack, line_number);

    (*stack)->n = sum;
}

void sub(stack_t **stack, unsigned int line_number) {
    int difference;

    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't sub, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    difference = (*stack)->next->n - (*stack)->n;
    pop(stack, line_number);

    (*stack)->n = difference;
}

void mul(stack_t **stack, unsigned int line_number) {
    int sum;

    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "Error: not enough elements in the stack to add\n");
        exit(EXIT_FAILURE);
    }

    sum = (*stack)->n * (*stack)->next->n;
    pop(stack, line_number);

    (*stack)->n = sum;
}
void perform_division(stack_t **stack, unsigned int line_number) {
    int result;

    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't div, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n == 0) {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }

    result = (*stack)->next->n / (*stack)->n;
    pop(stack, line_number);
    (*stack)->n = result;
}


void mod(stack_t **stack, unsigned int line_number) {
    int result;
    if (*stack == NULL || (*stack)->next == NULL) {
        fprintf(stderr, "L%u: can't mod, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n == 0) {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }

    result = (*stack)->next->n % (*stack)->n;
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
    stack_t *current = *stack;

    (void)line_number;
    if (*stack == NULL) {
        putchar('\n');
        return;
    }


    while (current != NULL && current->n != 0 && (current->n >= 0 && current->n <= 127)) {
        printf("%c",current->n);
        current = current->next;
    }

    putchar('\n');
}

void rotl(stack_t **stack, unsigned int line_number) {
    stack_t *current = *stack;

    (void)line_number;
    if (*stack == NULL || (*stack)->next == NULL) {
        return;
    }


    while (current->next != NULL) {
        current = current->next;
    }
    current->next = *stack;
    (*stack)->prev = current;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    current->next->next = NULL;
}

void rotr(stack_t **stack, unsigned int line_number) {
    stack_t *current = *stack;
    (void)line_number;

    if (*stack == NULL || (*stack)->next == NULL) {
                return;
    }



    while (current->next != NULL) {
        current = current->next;
    }
    current->prev->next = NULL;
    current->prev = NULL;
    (*stack)->prev = current;
    current->next = *stack;
    current->prev = NULL;
    *stack = current;
}
