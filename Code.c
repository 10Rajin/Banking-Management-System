#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FILE_NAME "accounts.txt"

void deposit_money(int *balance, int accountId);
void withdraw_money(int *balance, int accountId);
void menu(int accountId);
void viewTransactionHistory(int accountId);
int dip_amt, with_amt;

struct Account {
    int id;
    char username[30];
    char password[30];
};

// Function to get the last account ID from file
int getLastID() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;
    int lastID = 1000; // Start from 1000 for admin

    if (fp != NULL) {
        while (fscanf(fp, "%d %s %s", &acc.id, acc.username, acc.password) == 3) {
            lastID = acc.id;
        }
        fclose(fp);
    }
    return lastID;
}

// Create new account
void createAccount() {
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Account acc;
    printf("Enter username: ");
    scanf("%s", acc.username);
    printf("Enter password: ");
    scanf("%s", acc.password);

    acc.id = getLastID() + 1;

    fprintf(fp, "%d %s %s\n", acc.id, acc.username, acc.password);
    fclose(fp);

    printf("Account created successfully!\n");
    printf("Your Account ID is: %d (Remember this for login)\n", acc.id);
}

// Login
void login() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("No accounts found. Please create an account first.\n");
        return;
    }

    int inputID;
    char inputPass[30];
    struct Account acc;
    int found = 0;

    printf("Enter Account ID: ");
    scanf("%d", &inputID);
    printf("Enter Password: ");
    scanf("%s", inputPass);

    while (fscanf(fp, "%d %s %s", &acc.id, acc.username, acc.password) == 3) {
        if (acc.id == inputID && strcmp(acc.password, inputPass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("Login successful! Welcome, %s.\n", acc.username);
        menu(inputID);    
    } else {
        printf("Invalid Account ID or Password.\n");
    }
}

void menu(int accountId) {
    int balance; 
    int choice;
    while (1) {
        printf("\n===== Menu =====\n");
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Transaction History\n");
        printf("4. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                deposit_money(&balance, accountId);
                break;
            case 2:
                withdraw_money(&balance, accountId);
                break;
            case 3:
               	viewTransactionHistory(accountId);
               	break;
            case 4:
            	printf("Logging out...");
                return; // Exit the menu
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n===== Simple Banking Login System =====\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void deposit_money(int *balance, int accountId) {
    time_t tm;
    time(&tm);
    char filename[20];
    sprintf(filename, "transactions_%d.txt", accountId);
    FILE *ptr = fopen(filename, "a");
    if (ptr == NULL) {
        printf("Error opening file for deposit.\n");
        return;
    }

    printf("DEPOSITING MONEY\n");
    for (int i = 0; i < 50; i++) {
        printf("-");
    }

    printf("\nEnter the amount you want to deposit: ");
    scanf("%d", &dip_amt);
    *balance += dip_amt;
    printf("Money Deposited\n");
    printf("Now balance: %d\n", *balance);
    fprintf(ptr, "\nRs%d had been deposited to your account \n", dip_amt);
    fprintf(ptr, "Date/Time of transaction: %s\n", ctime(&tm));
    fclose(ptr);
    printf("Press Enter to continue...\n");
    getchar(); // To consume the newline character left by scanf
    getchar(); // Wait for user input
}

void withdraw_money(int *balance, int accountId) {
    time_t tm;
    time(&tm);
    char filename[20];
    sprintf(filename, "transactions_%d.txt", accountId);
    FILE *ptr = fopen(filename, "a");
    if (ptr == NULL) {
        printf("Error opening file for withdrawal.\n");
        return;
    }

    printf("WITHDRAWING MONEY\n");
    for (int i = 0; i < 50; i++) {
        printf("-");
    }

    printf("\nEnter the amount you want to withdraw: ");
    scanf("%d", &with_amt);

    if (*balance < with_amt) {
        printf("Insufficient balance\n");
    } else {
        *balance -= with_amt;
        printf("Money withdrawn\n");
        printf("Current balance: %d\n", *balance);
        fprintf(ptr, "\nRs%d had been withdrawn from your account \n", with_amt);
        fprintf(ptr, "Date/Time of transaction: %s\n", ctime(&tm));
    }
    fclose(ptr);
    printf("Press Enter to continue...\n");
    getchar(); // To consume the newline character left by scanf
    getchar(); // Wait for user input
}
void viewTransactionHistory(int accountId) {
    char filename[20];
    sprintf(filename, "transactions_%d.txt", accountId); // Corrected filename format

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No transaction history found for this account.\n");
        return;
    }

    char line[100];
    printf("\n--- Transaction History for Account %d ---\n", accountId);
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
}