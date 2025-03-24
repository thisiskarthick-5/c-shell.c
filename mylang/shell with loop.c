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

// Get variable value
double getVariableValue(char *name) {
    int index = findVariable(name);
    return (index != -1) ? variables[index].value : atof(name);
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

// Function to evaluate math operations
double evaluateExpression(char *expr) {
    double num1, num2;
    char op;

    // Check for math operations
    if (sscanf(expr, "%lf %c %lf", &num1, &op, &num2) == 3) {
        switch (op) {
            case '+': return num1 + num2;
            case '-': return num1 - num2;
            case '*': return num1 * num2;
            case '/': return (num2 != 0) ? num1 / num2 : 0; // Prevent division by zero
            default: printf(RED " ❌ Unsupported operation!\n" RESET); return 0;
        }
    }

    return getVariableValue(expr); // Return variable value or direct number
}

// Execute Mylang commands (with math support)
void executeMylang(char *command) {
    char var[20], expr[50];

    // Variable Assignments (Now includes math operations)
    if (sscanf(command, "%s = %[^\n]", var, expr) == 2) {
        double value = evaluateExpression(expr);
        setVariable(var, value);
        printf(GREEN " ✅ Assigned: %s = %.2f\n" RESET, var, value);
    }
    // Print Command
    else if (strncmp(command, "print ", 6) == 0) {
        sscanf(command + 6, "%s", var);
        printf(GREEN " 🔢 %.2f\n" RESET, getVariableValue(var));
    }
    else {
        printf(RED " ❌ Unknown command!\n" RESET);
    }
}

// Chatbot Response
void chatbotResponse(char *command) {
    if (strcmp(command, "hello") == 0) printf(CYAN " 🤖 Hello! How can I assist you?\n" RESET);
    else printf(CYAN " 🤖 I don’t understand. Try again!\n" RESET);
}

// File Operations
void fileOperations() {
    int choice;
    char filename[50], content[500], ch;

    while (1) {
        printf(YELLOW "\n📂 File Operations:\n" RESET);
        printf("1. Create File\n2. Read File\n3. Delete File\n4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter file name: ");
                scanf("%s", filename);
                getchar();
                printf("Enter content: ");
                fgets(content, sizeof(content), stdin);
                FILE *fptr = fopen(filename, "w");
                fprintf(fptr, "%s", content);
                fclose(fptr);
                printf(GREEN " ✅ File Created Successfully!\n" RESET);
                break;
            case 2:
                printf("Enter file name: ");
                scanf("%s", filename);
                getchar();
                fptr = fopen(filename, "r");
                if (fptr == NULL) {
                    printf(RED " ❌ File Not Found!\n" RESET);
                } else {
                    while ((ch = fgetc(fptr)) != EOF) putchar(ch);
                    fclose(fptr);
                }
                break;
            case 3:
                printf("Enter file name: ");
                scanf("%s", filename);
                getchar();
                if (remove(filename) == 0) printf(GREEN " ✅ File Deleted!\n" RESET);
                else printf(RED " ❌ File Not Found!\n" RESET);
                break;
            case 4:
                return;
            default:
                printf(RED " ❌ Invalid choice!\n" RESET);
        }
    }
}

// C Compiler Execution
void compileAndRunC() {
    char filename[50] = "temp.c";
    char command[200];

    printf("Enter C code (end with 'EOF'):\n");
    FILE *fptr = fopen(filename, "w");
    char line[100];

    while (1) {
        fgets(line, sizeof(line), stdin);
        if (strncmp(line, "EOF", 3) == 0) break;
        fputs(line, fptr);
    }
    fclose(fptr);

    sprintf(command, "gcc %s -o temp.out && ./temp.out", filename);
    system(command);
    remove("temp.out");
}

// Menu System
void menu() {
    int choice;
    char command[200];

    while (1) {
        printf(YELLOW "\n🚀 Welcome to the Interactive Shell 01!\n" RESET);
        printf("----------------------------------\n");
        printf("1️⃣ Mylang (Variables, Math, Print)\n");
        printf("2️⃣ Chatbot\n");
        printf("3️⃣ File Operations\n");
        printf("4️⃣ C Compiler\n");
        printf("5️⃣ Exit\n");
        printf("----------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                while (1) {
                    printf(BLUE "\n01> " RESET);
                    fgets(command, sizeof(command), stdin);
                    command[strcspn(command, "\n")] = 0;
                    if (strcmp(command, "exit") == 0) break;
                    executeMylang(command);
                }
                break;
            case 2:
                while (1) {
                    printf(CYAN "\nChatbot> " RESET);
                    fgets(command, sizeof(command), stdin);
                    command[strcspn(command, "\n")] = 0;
                    if (strcmp(command, "exit") == 0) break;
                    chatbotResponse(command);
                }
                break;
            case 3:
                fileOperations();
                break;
            case 4:
                compileAndRunC();
                break;
            case 5:
                printf(YELLOW "Goodbye! 👋\n" RESET);
                return;
            default:
                printf(RED " ❌ Invalid choice!\n" RESET);
        }
    }
}

int main() {
    menu();
    return 0;
}
