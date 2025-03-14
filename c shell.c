#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 200
#define MAX_VARS 100

// Structure to store variables
typedef struct {
    char name[50];
    int value;
} Variable;

Variable vars[MAX_VARS];
int varCount = 0;

// Function to get variable value
int getVarValue(char *varName) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, varName) == 0) {
            return vars[i].value;
        }
    }
    return -1; // Variable not found
}

// Function to set a variable
void setVarValue(char *varName, int value) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, varName) == 0) {
            vars[i].value = value;
            return;
        }
    }
    strcpy(vars[varCount].name, varName);
    vars[varCount].value = value;
    varCount++;
}

// Function to evaluate simple math expressions
int evaluateExpression(char *expr) {
    int num1, num2, result;
    char op;

    // Check for variable usage
    char var1[50], var2[50];
    if (sscanf(expr, "%s %c %s", var1, &op, var2) == 3) {
        int val1 = isdigit(var1[0]) ? atoi(var1) : getVarValue(var1);
        int val2 = isdigit(var2[0]) ? atoi(var2) : getVarValue(var2);

        if (val1 == -1 || val2 == -1) {
            printf("❌ Error: Undefined variable in expression.\n");
            return 0;
        }

        num1 = val1;
        num2 = val2;
    } else {
        sscanf(expr, "%d %c %d", &num1, &op, &num2);
    }

    switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if (num2 == 0) {
                printf("❌ Error: Division by zero.\n");
                return 0;
            }
            result = num1 / num2;
            break;
        default:
            printf("❌ Error: Unsupported operator.\n");
            return 0;
    }

    return result;
}

// Function to execute "run" command
void executeRunCommand(char *input) {
    char command[MAX_INPUT];

    // If input is enclosed in quotes, print text
    if (sscanf(input, "run \"%[^\"]\"", command) == 1) {
        printf("%s\n", command);
    } 
    // If it's a math expression, evaluate it
    else if (strpbrk(input, "+-*/") != NULL) {
        char expr[MAX_INPUT];
        sscanf(input, "run %[^\n]", expr);
        printf("%d\n", evaluateExpression(expr));
    } 
    // If it's a variable, print its value
    else if (sscanf(input, "run %s", command) == 1) {
        int value = getVarValue(command);
        if (value != -1) {
            printf("%d\n", value);
        } 
        // Otherwise, execute as a system command
        else {
            system(command);
        }
    } 
    else {
        printf("❌ Error: Invalid syntax. Use run \"message\", run math expression, or run command.\n");
    }
}

// Function to execute KScript commands
int executeKScript(char *input) {
    char arg1[50];
    int num1;

    // Handling "run" command
    if (strncmp(input, "run ", 4) == 0) {
        executeRunCommand(input);
        return 1;
    }

    // Handling "let" command for variable declaration
    if (sscanf(input, "let %s = %d", arg1, &num1) == 2) {
        setVarValue(arg1, num1);
        return 1;
    }

    return 0; // Not a KScript command
}

// Function to execute system commands
void executeShellCommand(char *input) {
    char *args[MAX_INPUT];
    char *token = strtok(input, " ");
    int argCount = 0;

    while (token != NULL) {
        args[argCount++] = token;
        token = strtok(NULL, " ");
    }
    args[argCount] = NULL;

    if (fork() == 0) {
        execvp(args[0], args);
        perror("Command execution failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

// Main Shell Loop
int main() {
    char input[MAX_INPUT];

    while (1) {
        printf("kshell> ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline

        if (strcmp(input, "exit") == 0) {
            printf("Exiting KShell...\n");
            break;
        }

        if (!executeKScript(input)) {
            executeShellCommand(input);
        }
    }

    return 0;
}
