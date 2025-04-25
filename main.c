#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FILE_NAME "expenses.csv"

//structure to hold expense data
typedef struct 
{
    char name[100];
    double amount;
    char date[20];
} Expense;

// Function to add an expense
void addExpense(Expense expense) 
{
    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) 
    {
        printf("Could not open file for writing.\n");
        return;
    }
    expense.name[strcspn(expense.name, "\n")] = '\0';
    expense.date[strcspn(expense.date, "\n")] = '\0';


    fprintf(file, "%s,%.2f,%s\n", expense.name, expense.amount, expense.date);
    fclose(file);
    printf("Expense added successfully.\n");
}
// Views all expenses and displays total
void viewExpenses()
 {

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) 
    {
        printf("Could not open file to view expenses.\n");
        return;
    }
    char name[100], date[20];
    double amount, total = 0.0;

    printf("\n%-25s %-10s %-15s\n", "Expense Name", "Amount", "Date");
    printf("-----------------------------------------------------------\n");

    while (fscanf(file, "%99[^,],%lf,%19[^\n]\n", name, &amount, date) == 3) 
    {
        printf("%-25s %-10.2f %-15s\n", name, amount, date);
        total += amount;
    }

    printf("\nTotal Expenses: %.2f\n", total);
    fclose(file);
}



    // Edits an expense
void editExpense(char *oldName, Expense newExpense) 
{

    oldName[strcspn(oldName, "\n")] = '\0';
    newExpense.name[strcspn(newExpense.name, "\n")] = '\0';
    newExpense.date[strcspn(newExpense.date, "\n")] = '\0';

    FILE *file = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!file || !temp) 
    {
        printf("Could not open file to edit expense.\n");
        return;
    }

    char n[100], d[20];
    double a;
    int found = 0;

    while (fscanf(file, "%99[^,],%lf,%19[^\n]\n", n, &a, d) == 3)
  {
        if (strcmp(n, oldName) == 0) 
        {
            fprintf(temp, "%s,%.2f,%s\n", newExpense.name, newExpense.amount, newExpense.date);
            found = 1;
        } 
        else 
        {
            fprintf(temp, "%s,%.2f,%s\n", n, a, d);
        }
    }

    fclose(file);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    if (found) 
    {
        printf("Expense updated.\n");
    } 
    else
    {
        printf("Expense not found.\n");
    }
}

    
// Function to delete an expense
void deleteExpense(char *name) 
{  
     name[strcspn(name, "\n")] = '\0';

    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.csv", "w");
    if (file == NULL || tempFile == NULL) 
    {
        printf("Error opening file.\n");
        return;
    }
    char line[256], tempName[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) 
    {   
        sscanf(line, "%99[^,],", tempName);
        if (strcmp(tempName, name)==0) 
        {
            found = 1; // Skip this expense
            continue;
        }
        fputs(line, tempFile);
    }
    fclose(file);
    fclose(tempFile);
    if (found)
    {
        remove(FILE_NAME);       
        rename("temp.csv", FILE_NAME); 
        printf("Expense deleted.\n");
    }
     else 
    {
        printf("Expense not found.\n");
    }
}

char* strptime_ymd(const char* s, const char* format, struct tm* tm) 
{
    if (strcmp(format, "%Y-%m-%d") != 0) 
    {
        return NULL;
    }

    int year, month, day;
    if (sscanf(s, "%d-%d-%d", &year, &month, &day) != 3)
 {
        return NULL;
    }

    tm->tm_year = year - 1900;
    tm->tm_mon = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1;

    return (char*)(s + strlen(s));
}


// Report of current week
void showWeeklyReport()
 {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    time_t startOfWeek = now - (tm_now->tm_wday * 86400);

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) 
    {
        printf("Could not open file for weekly report.\n");
        return;
    }

    char name[100], date[20];
    double amount, total = 0.0;
    struct tm entry = {0};

    printf("\nWeekly Report:\n");
    printf("%-25s %-10s %-15s\n", "Expense Name", "Amount", "Date");
    printf("-----------------------------------------------------------\n");

    while (fscanf(file, "%99[^,],%lf,%19[^\n]\n", name, &amount, date) == 3) 
    {
        memset(&entry, 0, sizeof(struct tm));
           if (strptime_ymd(date, "%Y-%m-%d", &entry)) 
           {
               time_t entryTime = mktime(&entry);
               if (difftime(entryTime, startOfWeek) >= 0) 
               {
                   printf("%-25s %-10.2f %-15s\n", name, amount, date);
                   total += amount;
               }
           }
       }
   
    printf("\nTotal for This Week: %.2f\n", total);
    fclose(file);
}


int main() 
{
    int choice;
    Expense expense;
    char oldName[100];

    printf("Welcome to the Expense Tracker\n");

    while (1)
   {
    printf("1. Add Expense\n");
    printf("2. View Expenses\n");
    printf("3. Edit Expense\n");
    printf("4. Delete Expense\n");
    printf("5. Show weekly Report\n");
    printf("6. Exit\n");

    
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (choice) 
        {
            case 1:
                printf("Enter expense name: ");
                fgets(expense.name, sizeof(expense.name), stdin);
                printf("Enter expense amount: ");
                scanf("%lf", &expense.amount);
                getchar();  
                printf("Enter expense date (YYYY-MM-DD): ");
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
                getchar();
                printf("Enter new expense date (YYYY-MM-DD): ");
                fgets(expense.date, sizeof(expense.date), stdin);
                editExpense(oldName, expense);
                break;
            case 4:
                printf("Enter the name of the expense to delete: ");
                fgets(oldName, sizeof(oldName), stdin);
                deleteExpense(oldName);
                break;
            case 5:
                showWeeklyReport();
                break;    
            case 6:
                printf("Exiting...\nThank you for using the Expense Tracker!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}