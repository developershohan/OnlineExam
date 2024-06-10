#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void load_users(User *users, int *num_users) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        // File may not exist initially, so we handle this gracefully
        *num_users = 0;
        return;
    }

    char line[USERNAME_LENGTH + PASSWORD_LENGTH + 2];
    int index = 0;

    while (fgets(line, sizeof(line), file) && index < MAX_USERS) {
        sscanf(line, "%s %s", users[index].username, users[index].password);
        index++;
    }

    *num_users = index;
    fclose(file);
}

int register_user(User *users, int *num_users) {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];

    printf("Register a new account\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < *num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Try another one.\n");
            return 0;
        }
    }

    strcpy(users[*num_users].username, username);
    strcpy(users[*num_users].password, password);
    (*num_users)++;

    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("Registration successful!\n");
    return 1;  // Indicates successful registration
}

int login_user(User *users, int num_users) {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];

    printf("Login\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            return 1;  // Login successful
        }
    }

    printf("Invalid username or password.\n");
    return 0;  // Login failed
}
