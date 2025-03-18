#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

typedef struct {
    char name[20];
    double value;
} Variable;

Variable variables[100];
int varCount = 0;

// Trim spaces
void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

// Find variable
int findVariable(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) return i;
    }
    return -1;
}

// Set variable
void setVariable(char *name, double value) {
    int index = findVariable(name);
    if (index == -1) {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
    } else {
        variables[index].value = value;
    }
}

// Process math operations
void mathOperation(char *command) {
    char op[10];
    double num1, num2, result;

    if (sscanf(command, "%s %lf %lf", op, &num1, &num2) < 2) {
        printf(RED " ❌ Use: math [add/sub/mul/div] [num1] [num2]\n" RESET);
        return;
    }

    if (strcmp(op, "add") == 0) result = num1 + num2;
    else if (strcmp(op, "sub") == 0) result = num1 - num2;
    else if (strcmp(op, "mul") == 0) result = num1 * num2;
    else if (strcmp(op, "div") == 0) {
        if (num2 == 0) {
            printf(RED " ❌ Cannot divide by zero!\n" RESET);
            return;
        }
        result = num1 / num2;
    } else {
        printf(RED " ❌ Invalid operation!\n" RESET);
        return;
    }

    printf(GREEN " ✅ Result: %.2f\n" RESET, result);
}

// MYLANG: Execute commands
void executeMylang(char *command) {
    char var[20], eq[2], expr[50];

    // Handling Variable Assignments (e.g., x = 10 + 5)
    if (sscanf(command, "%s %s %[^\n]", var, eq, expr) == 3 && strcmp(eq, "=") == 0) {
        double value = 0;
        char op;
        double num1, num2;

        // Check if it's an arithmetic operation
        if (sscanf(expr, "%lf %c %lf", &num1, &op, &num2) == 3) {
            if (op == '+') value = num1 + num2;
            else if (op == '-') value = num1 - num2;
            else if (op == '*') value = num1 * num2;
            else if (op == '/') value = (num2 != 0) ? num1 / num2 : 0;
            else {
                printf(RED " ❌ Invalid operator!\n" RESET);
                return;
            }
        } else {
            value = atof(expr);
        }

        setVariable(var, value);
        printf(GREEN " ✅ Assigned: %s = %.2f\n" RESET, var, value);
    }
    // Handling Print Command
    else if (strncmp(command, "print ", 6) == 0) {
        char varName[20];
        sscanf(command + 6, "%s", varName);
        int index = findVariable(varName);

        if (index != -1) {
            printf(GREEN " 🔢 %.2f\n" RESET, variables[index].value);
        } else {
            printf(RED " ❌ Variable '%s' not found!\n" RESET);
        }
    }
    // Handling Loops (e.g., loop 3 { print x })
    else if (strncmp(command, "loop ", 5) == 0) {
        int times;
        char loopContent[200];

        // Extracting loop count and body
        if (sscanf(command, "loop %d { %[^\n] }", &times, loopContent) == 2 && times > 0) {
            for (int i = 0; i < times; i++) {
                executeMylang(loopContent);  // Execute loop body without showing iteration count
            }
        } else {
            printf(RED " ❌ Invalid loop syntax! Use: loop N { command }\n" RESET);
        }
    }
    // Invalid Command
    else {
        printf(RED " ❌ Unknown Mylang command!\n" RESET);
    }
}


// AI Chat Mode
void chatMode() {
    char input[100];
    printf(CYAN " 🤖 AI Chat Mode! Type 'exit' to leave.\n" RESET);

    while (1) {
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf(YELLOW " 🔄 Exiting Chat Mode...\n" RESET);
            break;
        } else {
            printf(GREEN " 🤖 AI: I'm still learning!\n" RESET);
        }
    }
}

// Compiler Mode
void compilerMode() {
    char code[5000] = "";
    char input[200];
    FILE *file;

    printf(CYAN " ✨ Compiler Mode! Type 'run' to compile & execute.\n" RESET);

    while (1) {
        printf(BLUE "C> " RESET);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf(YELLOW " 🔄 Exiting Compiler Mode...\n" RESET);
            break;
        } else if (strcmp(input, "run") == 0) {
            file = fopen("temp_code.c", "w");
            if (!file) {
                printf(RED " ❌ Error creating temp file!\n" RESET);
                return;
            }
            fprintf(file, "%s", code);
            fclose(file);
            system("gcc temp_code.c -o temp_code.out 2> temp_errors.txt");
            if (access("temp_code.out", F_OK) == 0) {
                printf(GREEN " ✅ Compilation successful! Running program...\n" RESET);
                system("./temp_code.out");
            } else {
                printf(RED " ❌ Compilation failed! Errors:\n" RESET);
                system("cat temp_errors.txt");
            }
        } else {
            strcat(code, input);
            strcat(code, "\n");
        }
    }
}

// Process command
void processCommand(char *command) {
    trimSpaces(command);

    if (strncmp(command, "math ", 5) == 0) {
        mathOperation(command + 5);
    } else if (strcmp(command, "chat") == 0) {
        chatMode();
    } else if (strcmp(command, "compiler") == 0) {
        compilerMode();
    } else if (strncmp(command, "mylang ", 7) == 0) {
        executeMylang(command + 7);
    } else if (strcmp(command, "exit") == 0) {
        printf(YELLOW " 🔥 Exiting Karthick Shell...\n" RESET);
        exit(0);
    } else {
        printf(RED " ❌ Unknown command!\n" RESET);
    }
}

// Main function
int main() {
    char command[100];

    printf(YELLOW " ╔════════════════════════════════════╗\n" RESET);
    printf(YELLOW " ║ 🚀 Welcome to Karthick Shell ║\n" RESET);
    printf(YELLOW " ╠════════════════════════════════════╣\n" RESET);
    printf(YELLOW " ║ 🤖 chat (AI Chat Mode) ║\n" RESET);
    printf(YELLOW " ║ 🖥 compiler (C Compiler Mode) ║\n" RESET);
    printf(YELLOW " ║ 🔣 mylang [command] (Your Language)║\n" RESET);
    printf(YELLOW " ╚════════════════════════════════════╝\n" RESET);

    while (1) {
        printf(BLUE "\n>> " RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        processCommand(command);
    }

    return 0;
}
