#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    memmove(str, start, end - start + 2);
}

void createFile(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
        printf(GREEN " ✅ File Created: %s\n" RESET, filename);
    } else {
        printf(RED " ❌ Error creating file: %s\n" RESET, filename);
    }
}

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

void compilerMode() {
    char code[5000] = "";
    char input[200];
    FILE *file;
    
    printf(CYAN " ✨ Compiler Mode Activated! Type 'run' to compile & execute.\n" RESET);
    printf(YELLOW " ✨ Type 'cmd sc' to exit compiler mode.\n" RESET);

    while (1) {
        printf(BLUE "C> " RESET);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "run") == 0) {
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
                printf(RED " ❌ Compilation failed! Check errors:\n" RESET);
                system("cat temp_errors.txt");
            }
        } else if (strcmp(input, "cmd sc") == 0) {
            printf(YELLOW " 🔄 Exiting Compiler Mode...\n" RESET);
            return;
        } else {
            strcat(code, input);
            strcat(code, "\n");
        }
    }
}

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
        } else {
            printf(GREEN " 🤖 AI: I'm learning!\n" RESET);
        }
    }
}

void processCommand(char *command) {
    trimSpaces(command);

    if (strncmp(command, "touch ", 6) == 0) {
        createFile(command + 6);
    } else if (strncmp(command, "cat ", 4) == 0) {
        readFile(command + 4);
    } else if (strncmp(command, "write ", 6) == 0) {
        writeFile(command + 6);
    } else if (strcmp(command, "compiler") == 0) {
        compilerMode();
    } else if (strcmp(command, "chat") == 0) {
        chatMode();
    } else if (strcmp(command, "exit") == 0) {
        printf(YELLOW " 🔥 Exiting Karthick Shell...\n" RESET);
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
    printf(YELLOW " ║ 📄 touch filename (Create File) ║\n" RESET);
    printf(YELLOW " ║ 📖 cat filename (Read File) ║\n" RESET);
    printf(YELLOW " ║ ✍️ write filename \"text\" (Write to File) ║\n" RESET);
    printf(YELLOW " ║ 🤖 chat (AI Chat Mode) ║\n" RESET);
    printf(YELLOW " ║ 💻 compiler (Start Compiler Mode) ║\n" RESET);
    printf(YELLOW " ║ 🔥 exit (Exit Shell) ║\n" RESET);
    printf(YELLOW " ╚════════════════════════════════════╝\n" RESET);

    while (1) {
        printf(BLUE "\n>> " RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        processCommand(command);
    }

    return 0;
}

