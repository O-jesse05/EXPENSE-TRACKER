#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "expenses.csv"

//structure to hold expense data
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
// Views all expenses and displays total
void viewExpenses() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Could not open file to view expenses.\n");
        return;
    }

    char name[100], date[20];
    double amount, total = 0.0;

    printf("\n%-25s %-10s %-15s\n", "Expense Name", "Amount", "Date");
    printf("-----------------------------------------------------------\n");

    while (fscanf(file, "%99[^,],%lf,%19[^\n]\n", name, &amount, date) == 3) {
        printf("%-25s %-10.2f %-15s\n", name, amount, date);
        total += amount;
    }

    printf("\nTotal Expenses: %.2f\n", total);
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


int main() {
    int choice;
    Expense expense;
    char oldName[100];

    printf("Welcome to the Expense Tracker\n");
    printf("1. Add Expense\n");
    printf("2. View Expenses\n");
    printf("3. Edit Expense\n");
    printf("4. Delete Expense\n");
    printf("5. Exit\n");

    while (1) {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume newline character left by scanf

        switch (choice) {
            case 1:
                printf("Enter expense name: ");
                fgets(expense.name, sizeof(expense.name), stdin);
                printf("Enter expense amount: ");
                scanf("%lf", &expense.amount);
                printf("Enter expense date (YYYY-MM-DD): ");
                getchar();  // To consume newline character left by scanf
                fgets(expense.date, sizeof(expense.date), stdin);
                addExpense(expense);
                break;
            case 2:
                viewExpenses();
                break;
            case 3:
                printf("Enter the name of the expense to edit: ");
                fgets(oldName, sizeof(oldName), stdin);
                printf("Enter new expense name: ");
                fgets(expense.name, sizeof(expense.name), stdin);
                printf("Enter new expense amount: ");
                scanf("%lf", &expense.amount);
                printf("Enter new expense date (YYYY-MM-DD): ");
                getchar();  // To consume newline character left by scanf
                fgets(expense.date, sizeof(expense.date), stdin);
                editExpense(oldName, expense);
                break;
            case 4:
                printf("Enter the name of the expense to delete: ");
                fgets(oldName, sizeof(oldName), stdin);
                deleteExpense(oldName);
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Function to edit an expense
void editExpense(char *oldName, Expense newExpense) {
    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL) {
        printf("Could not open file for editing.\n");
        return;
    }
    char line[256];
    long pos;
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        pos = ftell(file) - strlen(line);
        if (strstr(line, oldName)) {
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s,%.2f,%s\n", newExpense.name, newExpense.amount, newExpense.date);
            found = 1;
            break;
        }
    }
    if (found) {
        printf("Expense updated.\n");
    } else {
        printf("Expense not found.\n");
    }
    fclose(file);
}