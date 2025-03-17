#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_VARS 100

// ANSI color codes for styling
#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

// Function to trim spaces
void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    memmove(str, start, end - start + 2);
}

// Function to create a file
void createFile(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
        printf(GREEN " ✅ File Created: %s\n" RESET, filename);
    } else {
        printf(RED " ❌ Error creating file: %s\n" RESET, filename);
    }
}

// Function to read a file
void readFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(RED " ❌ Error opening file: %s\n" RESET, filename);
        return;
    }

    char ch;
    printf(YELLOW " 📖 Content of %s:\n" RESET, filename);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
    printf("\n");
}

// Function to write to a file
void writeFile(char *command) {
    char filename[50], content[200];

    if (sscanf(command, "%s \"%[^\"]\"", filename, content) != 2) {
        printf(RED " ❌ Invalid syntax! Use: write filename \"text\"\n" RESET);
        return;
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED " ❌ Error opening file: %s\n" RESET, filename);
        return;
    }

    fprintf(file, "%s\n", content);
    fclose(file);
    printf(GREEN " ✅ Text written to %s\n" RESET, filename);
}

// AI Chat Mode
void chatMode() {
    char input[100];
    printf(CYAN " 🤖 AI Chat Mode Activated! Type 'exit' to leave.\n" RESET);

    while (1) {
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf(YELLOW " 🔄 Exiting AI Chat Mode...\n" RESET);
            break;
        } else if (strcmp(input, "hello") == 0) {
            printf(GREEN " 🤖 AI: Hello! How can I assist you?\n" RESET);
        } else if (strcmp(input, "how are you?") == 0) {
            printf(GREEN " 🤖 AI: I'm just a shell program, but I'm doing great!\n" RESET);
        } else {
            printf(GREEN " 🤖 AI: I don't understand, but I'm learning!\n" RESET);
        }
    }
}

// Function to perform math operations
void mathOperation(char *command) {
    char operation[10];
    double num1, num2, result;

    if (sscanf(command, "%s %lf %lf", operation, &num1, &num2) < 2) {
        printf(RED " ❌ Invalid syntax! Use: math [operation] [num1] [num2]\n" RESET);
        return;
    }

    if (strcmp(operation, "add") == 0) {
        result = num1 + num2;
    } else if (strcmp(operation, "sub") == 0) {
        result = num1 - num2;
    } else if (strcmp(operation, "mul") == 0) {
        result = num1 * num2;
    } else if (strcmp(operation, "div") == 0) {
        if (num2 == 0) {
            printf(RED " ❌ Cannot divide by zero!\n" RESET);
            return;
        }
        result = num1 / num2;
    } else if (strcmp(operation, "mod") == 0) {
        result = fmod(num1, num2);
    } else if (strcmp(operation, "pow") == 0) {
        result = pow(num1, num2);
    } else if (strcmp(operation, "sqrt") == 0) {
        if (num1 < 0) {
            printf(RED " ❌ Cannot calculate square root of a negative number!\n" RESET);
            return;
        }
        result = sqrt(num1);
    } else {
        printf(RED " ❌ Unknown math operation!\n" RESET);
        return;
    }

    printf(GREEN " ✅ Result: %.2f\n" RESET, result);
}

// Function to process commands
void processCommand(char *command) {
    trimSpaces(command);

    if (strncmp(command, "touch ", 6) == 0) {
        createFile(command + 6);
    } else if (strncmp(command, "cat ", 4) == 0) {
        readFile(command + 4);
    } else if (strncmp(command, "write ", 6) == 0) {
        writeFile(command + 6);
    } else if (strcmp(command, "chat") == 0) {
        chatMode();
    } else if (strncmp(command, "math ", 5) == 0) {
        mathOperation(command + 5);
    } else if (strcmp(command, "exit") == 0) {
        printf(YELLOW " 🔥 Exiting Karthick Shell...\n" RESET);
        exit(0);
    } else {
        printf(RED " ❌ Unknown command.\n" RESET);
    }
}

// Main function
int main() {
    char command[100];

    printf(YELLOW " ╔════════════════════════════════════╗\n" RESET);
    printf(YELLOW " ║ 🚀 Welcome to " BLUE "Karthick Shell" YELLOW " ║\n" RESET);
    printf(YELLOW " ╠════════════════════════════════════╣\n" RESET);
    printf(YELLOW " ║ Commands: ║\n" RESET);
    printf(YELLOW " ║ 📄 touch filename (Create File) ║\n" RESET);
    printf(YELLOW " ║ 📖 cat filename (Read File) ║\n" RESET);
    printf(YELLOW " ║ ✍️ write filename \"text\" (Write to File) ║\n" RESET);
    printf(YELLOW " ║ 🤖 chat (AI Chat Mode) ║\n" RESET);
    printf(YELLOW " ║ ➕ math add num1 num2 (Addition) ║\n" RESET);
    printf(YELLOW " ║ ➖ math sub num1 num2 (Subtraction) ║\n" RESET);
    printf(YELLOW " ║ ✖️ math mul num1 num2 (Multiplication) ║\n" RESET);
    printf(YELLOW " ║ ➗ math div num1 num2 (Division) ║\n" RESET);
    printf(YELLOW " ║ 🏗️ math mod num1 num2 (Modulus) ║\n" RESET);
    printf(YELLOW " ║ 🔢 math pow num1 num2 (Power) ║\n" RESET);
    printf(YELLOW " ║ 📏 math sqrt num1 (Square Root) ║\n" RESET);
    printf(YELLOW " ╚════════════════════════════════════╝\n" RESET);

    while (1) {
        printf(BLUE "\n>> " RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        processCommand(command);
    }

    return 0;
}
