#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

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

// Function to trim spaces from a string
void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    memmove(str, start, end - start + 2);
}

// Function to create a new file
void createFile(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
        printf(GREEN " ✅ File Created: %s\n" RESET, filename);
    } else {
        printf(RED " ❌ Error creating file: %s\n" RESET, filename);
    }
}

// Function to delete a file
void deleteFile(char *filename) {
    if (remove(filename) == 0) {
        printf(GREEN " ✅ File Deleted: %s\n" RESET, filename);
    } else {
        printf(RED " ❌ Error deleting file: %s\n" RESET, filename);
    }
}

// Function to create a directory
void createDirectory(char *dirname) {
    if (mkdir(dirname, 0777) == 0) {
        printf(GREEN " ✅ Directory Created: %s\n" RESET, dirname);
    } else {
        printf(RED " ❌ Error creating directory: %s\n" RESET, dirname);
    }
}

// Function to delete a directory
void deleteDirectory(char *dirname) {
    if (rmdir(dirname) == 0) {
        printf(GREEN " ✅ Directory Deleted: %s\n" RESET, dirname);
    } else {
        printf(RED " ❌ Error deleting directory: %s\n" RESET, dirname);
    }
}

// Function to list files in the current directory
void listFiles() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        printf(RED " ❌ Error opening directory\n" RESET);
        return;
    }

    printf(CYAN " 📂 Files in Directory:\n" RESET);
    while ((entry = readdir(dir)) != NULL) {
        printf(" %s\n", entry->d_name);
    }
    closedir(dir);
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

// Function to process user commands
void processCommand(char *command) {
    trimSpaces(command);

    if (strncmp(command, "touch ", 6) == 0) {
        createFile(command + 6);
    } else if (strncmp(command, "rm ", 3) == 0) {
        deleteFile(command + 3);
    } else if (strncmp(command, "mkdir ", 6) == 0) {
        createDirectory(command + 6);
    } else if (strncmp(command, "rmdir ", 6) == 0) {
        deleteDirectory(command + 6);
    } else if (strcmp(command, "ls") == 0) {
        listFiles();
    } else if (strncmp(command, "cat ", 4) == 0) {
        readFile(command + 4);
    } else if (strncmp(command, "write ", 6) == 0) {
        writeFile(command + 6);
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
    printf(YELLOW " ║ 📂 ls (List Files) ║\n" RESET);
    printf(YELLOW " ║ 📁 mkdir dirname (Create Directory) ║\n" RESET);
    printf(YELLOW " ║ 🗑️ rmdir dirname (Remove Directory) ║\n" RESET);
    printf(YELLOW " ║ 📄 touch filename (Create File) ║\n" RESET);
    printf(YELLOW " ║ ❌ rm filename (Delete File) ║\n" RESET);
    printf(YELLOW " ║ 📖 cat filename (Read File) ║\n" RESET);
    printf(YELLOW " ║ ✍️ write filename \"text\" (Write to File) ║\n" RESET);
    printf(YELLOW " ╚════════════════════════════════════╝\n" RESET);

    while (1) {
        printf(BLUE "\n>> " RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        processCommand(command);
    }

    return 0;
}
