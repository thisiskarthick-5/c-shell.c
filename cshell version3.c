#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

#define MAX_VARS 100

// ANSI color codes for styling
#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

// Structure to store variables
typedef struct {
    char name[20];
    int value;
} Variable;

Variable variables[MAX_VARS];
int varCount = 0;

// Function to convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to get variable value
int getVariableValue(char *name, int *value) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            *value = variables[i].value;
            return 1;
        }
    }
    return 0; // Variable not found
}

// Function to set variable
void setVariable(char *name, int value) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value; // Update existing variable
            return;
        }
    }
    if (varCount < MAX_VARS) {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
        printf(YELLOW " ✅ Variable Set: " BLUE "%-10s" RESET " = " GREEN "%d\n" RESET, name, value);
    } else {
        printf(RED " ❌ Error: Variable limit reached!\n" RESET);
    }
}

// Function to trim spaces from a string
void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    memmove(str, start, end - start + 2);
}

// Function to process mathematical expressions
void processMath(char *expression) {
    trimSpaces(expression);

    char var1[20], var2[20];
    int num1, num2, result;
    char op;

    if (sscanf(expression, "%s %c %s", var1, &op, var2) != 3) {
        printf(RED " ❌ Syntax Error! Use: math var1 + var2\n" RESET);
        return;
    }

    if (!getVariableValue(var1, &num1)) num1 = atoi(var1);
    if (!getVariableValue(var2, &num2)) num2 = atoi(var2);

    switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if (num2 == 0) {
                printf(RED " ❌ Error: Division by zero!\n" RESET);
                return;
            }
            result = num1 / num2;
            break;
        case '%': 
            if (num2 == 0) {
                printf(RED " ❌ Error: Modulo by zero!\n" RESET);
                return;
            }
            result = num1 % num2;
            break;
        default:
            printf(RED " ❌ Invalid operator! Use +, -, *, /, %\n" RESET);
            return;
    }

    printf(GREEN " ✅ Result: %-10d\n" RESET, result);
}

// Function to execute loop
void processLoop(char *command) {
    int times;
    char loopCmd[100];

    if (sscanf(command, "%d %[^\n]", &times, loopCmd) != 2) {
        printf(RED " ❌ Invalid loop syntax! Use: loop times command\n" RESET);
        return;
    }

    printf(CYAN " 🔄 Running loop %d times:\n" RESET, times);
    for (int i = 0; i < times; i++) {
        processCommand(loopCmd);
    }
}

// Function to change directory
void changeDirectory(char *path) {
    if (chdir(path) == 0) {
        printf(GREEN " ✅ Changed directory to: %s\n" RESET, path);
    } else {
        perror(RED " ❌ Error");
    }
}

// Function to print the current working directory
void printWorkingDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(CYAN " 📂 Current Directory: %s\n" RESET, cwd);
    } else {
        perror(RED " ❌ Error getting directory");
    }
}

// Function to process user commands
void processCommand(char *command) {
    trimSpaces(command);
    toLowerCase(command);

    if (strncmp(command, "runk ", 5) == 0) {
        char varName[20];
        if (sscanf(command + 5, "%s", varName) == 1) {
            int value;
            if (getVariableValue(varName, &value)) {
                printf(GREEN " 🔍 %s = %d\n" RESET, varName, value);
            } else {
                printf("%s\n", command + 5);
            }
        }
    } else if (strncmp(command, "set ", 4) == 0) {
        char varName[20];
        int value;
        if (sscanf(command + 4, "%s %d", varName, &value) == 2) {
            setVariable(varName, value);
        } else {
            printf(RED " ❌ Invalid syntax! Use: set var value\n" RESET);
        }
    } else if (strncmp(command, "math ", 5) == 0) {
        processMath(command + 5);
    } else if (strncmp(command, "loop ", 5) == 0) {
        processLoop(command + 5);
    } else if (strncmp(command, "cd ", 3) == 0) {
        changeDirectory(command + 3);
    } else if (strcmp(command, "pwd") == 0) {
        printWorkingDirectory();
    } else if (strcmp(command, "exit") == 0) {
        printf(YELLOW " 🔥 Exiting " BLUE "Karthick Shell" YELLOW "...\n" RESET);
        exit(0);
    } else {
        printf(RED " ❌ Unknown command.\n" RESET);
    }
}

int main() {
    char command[100];

    printf(YELLOW " ╔════════════════════════════════════╗\n" RESET);
    printf(YELLOW " ║ 🚀 Welcome to " BLUE "Karthick Shell" YELLOW " ║\n" RESET);
    printf(YELLOW " ╠════════════════════════════════════╣\n" RESET);
    printf(YELLOW " ║ Commands: ║\n" RESET);
    printf(YELLOW " ║ 📝 set var value ║\n" RESET);
    printf(YELLOW " ║ 🔍 runk var ║\n" RESET);
    printf(YELLOW " ║ ➕ math var1 + var2 ║\n" RESET);
    printf(YELLOW " ║ 🔄 loop times command ║\n" RESET);
    printf(YELLOW " ║ 📂 cd path (Change Directory) ║\n" RESET);
    printf(YELLOW " ║ 🏠 pwd (Current Directory) ║\n" RESET);
    printf(YELLOW " ╚════════════════════════════════════╝\n" RESET);

    while (1) {
        printf(BLUE "\n>> " RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        processCommand(command);
    }

    return 0;
}
