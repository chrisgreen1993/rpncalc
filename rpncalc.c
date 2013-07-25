/*
 * rpncalc.c
 *
 * Reverse Polish Notation Calculator.
 *
 * Author: Chris Green
 * chrisgreen1993.com
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
/*
 * Error handler function.
 *
 * msg: message to print
 */
void errorHandler(char *msg) {

    printf("Error: %s\n", msg);
    exit(EXIT_FAILURE);
}
/*
 * Struct used as nodes in the stack.
 *
 * next contains pointer to next StackNode struct.
 */
typedef struct StackNode {
    double value;
    struct StackNode *next;
} StackNode;


StackNode *stackPtr = NULL; // Stack pointer, points to top of the stack.

/*
 * Creates a new StackNode struct.
 *
 * value: numeric value to go in value element.
 * next: pointer to next node in chain (previous stack element).
 *
 * return: pointer to created StackNode.
 */
StackNode* newNode(double value, StackNode *next) {

    StackNode *node = malloc(sizeof(StackNode));
    node->value = value;
    node->next = next;
    return node;
}

/*
 * Pushes a new element onto stack.
 * Uses newNode function to create a new element, setting next pointer to current node at stackPtr.
 * Updates stackPtr to point to this node.
 *
 * value: numeric value to go in value element.
 */
void push(double value) {

    stackPtr = newNode(value, stackPtr);

}

/*
 * Pops top element from stack.
 * Checks if stack isn't empty and sets stackPtr to its next element.
 *
 * return: value from StackNode or calls errorHandler if stack is empty.
 */
double pop() {

    if (stackPtr != NULL) {
        double value = stackPtr->value;
        stackPtr = stackPtr->next;
        return value;
    }
    errorHandler("Stack underflow");
}

// Function pointer for operation functions.
typedef double (*opFunction)(double, double);

//array of operations.
const char *operations[] = {"+", "-", "*", "/", "^"};

// Calculates number of elements in operations array.
const int numOps = sizeof(operations)/sizeof(operations[0]);

/*
 * Adds val1 to val2.
 *
 * return: result of val1 + val2.
 */
double add(double val1, double val2) {

    return val1 + val2;
}

/*
 * Subtracts val1 to val2.
 *
 * return: result of val1 - val2.
 */
double sub(double val1, double val2) {

    return val1 - val2;
}

/*
 * Multiplies val1 by val2.
 *
 * return: result of val1 * val2.
 */
double mul(double val1, double val2) {

    return val1 * val2;
}

/*
 * Divides val1 by val2.
 *
 * return: result of val1 / val2.
 */
double divi(double val1, double val2) {

    return val1 / val2;
}

/*
 * val1 raised to power val2.
 * Uses math.h pow() function.
 *
 * val1: base
 * val2: exponent
 *
 * return: result of val1 ^ val2.
 */
double powr(double val1, double val2) {

    return pow(val1, val2);
}

// Array for function pointer, addresses of operation functions.
opFunction opFunctions[] = {&add, &sub, &mul, &divi, &powr};

/*
 * Checks if tokenised string is valid double.
 *
 * str: string to check.
 *
 * return: true if valid double, false otherwise.
 */
bool isDouble(char *str) {

    char *end; // Set to next character in str after numerical value.
    strtod(str, &end);
    if (end == str || *end != '\0') {
        return false;
    }
    return true;
}

/*
 * Tokenises input string, pushes onto stack if value is double.
 * If value is operation then pop two values from stack and call relevent operation function.
 *
 * str: string to evaluate (argv[1]).
 *
 * return: Final value left on stack (the result), or if there is more than one value left, call errorHandler.
 */
double evaluate(char *str) {

    char *p = strtok(str, " "); // Used to tokenise string.
    while (p != NULL) {
        if (isDouble(p)){
            push(strtod(p, NULL)); // strtod converts string to double.
        } else {
            int i;
            for (i = 0; i < numOps; i++) {
                if (!strcmp(p, operations[i])) { // Compares operation against array of ops to select correct one.
                    double val2 = pop();
                    double val1 = pop(); // Pop two values.
                    double res = (*opFunctions[i])(val1, val2); // Function pointer array is used to call correct one.
                    push(res); // Rseult is pushed to stack.
                }
            }
        }
        p = strtok(NULL, " ");
    }
    if (stackPtr->next == NULL) { // Check to see if last element on stack.
        return pop();
    } else {
        errorHandler("Incorrect format");
    }
    errorHandler("Unknown");
}

/*
 * Main function, checks to see if argument count is correct, passes argv[1] into evaluate function and prints result.
 *
 * argc: Argument count.
 * argv: Array of arguments.
 *
 * return: 0 if successful, otherwise 1.
 */
int main(int argc, char *argv[]) {

    if (argc == 2) {
        double result = evaluate(argv[1]);
        printf("Answer: %f\n", result);
        return 0;
    }
    return 1;
}
