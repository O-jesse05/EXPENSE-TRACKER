#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "expenses.csv"

typedef struct {
    char name[100];
    double amount;
    char date[20];
} Expense;

// Function to add an expense
void addExpense(Expense expense) {
    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Could not open file for writing.\n");
        return;
    }
    fprintf(file, "%s,%.2f,%s\n", expense.name, expense.amount, expense.date);
    fclose(file);
    printf("Expense added successfully.\n");
}

// Function to view all expenses
void viewExpenses() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Could not open file for reading.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}
// Function to delete an expense
void deleteExpense(char *name) {
    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.csv", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, name)) {
            found = 1; // Skip this expense
            continue;
        }
        fputs(line, tempFile);
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(FILE_NAME);       
        rename("temp.csv", FILE_NAME); 
        printf("Expense deleted.\n");
    } else {
        printf("Expense not found.\n");
    }
}