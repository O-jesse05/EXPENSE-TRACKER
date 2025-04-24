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
