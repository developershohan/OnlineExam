#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_QUESTIONS 100
#define MAX_OPTIONS 4
#define MAX_LINE_LENGTH 256
#define MAX_USERS 100
#define USERNAME_LENGTH 50
#define PASSWORD_LENGTH 50

typedef struct {
    char question[MAX_LINE_LENGTH];
    char options[MAX_OPTIONS][MAX_LINE_LENGTH];
    char correct_option;
} Question;

typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;

void load_questions(Question *questions, int *num_questions) {
    FILE *file = fopen("questions.txt", "r");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int index = 0;

    while (fgets(line, sizeof(line), file) && index < MAX_QUESTIONS) {
        strcpy(questions[index].question, line);

        for (int i = 0; i < MAX_OPTIONS; i++) {
            fgets(questions[index].options[i], sizeof(line), file);
        }

        fgets(line, sizeof(line), file);
        questions[index].correct_option = line[0];
        index++;
    }

    *num_questions = index;
    fclose(file);
}

void shuffle_questions(Question *questions, int num_questions) {
    srand(time(NULL));
    for (int i = num_questions - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
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
    return 1;
}

int login_user(User *users, int num_users) {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];

    printf("Log in to your account\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            return i; // Return the index of the logged-in user
        }
    }

    printf("Invalid username or password.\n");
    return -1; // Indicate login failure
}

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

void save_record(const char *username, int score, int num_questions) {
    FILE *file = fopen("records.txt", "a");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d/%d\n", username, score, num_questions);
    fclose(file);
}

void view_records(const char *username) {
    FILE *file = fopen("records.txt", "r");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    printf("Previous records for %s:\n", username);
    while (fgets(line, sizeof(line), file)) {
        char record_username[USERNAME_LENGTH];
        int score, num_questions;

        sscanf(line, "%s %d/%d", record_username, &score, &num_questions);
        if (strcmp(record_username, username) == 0) {
            printf("%s\n", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No records found.\n");
    }

    fclose(file);
}

void conduct_exam(Question *questions, int num_questions, int time_limit, const char *username) {
    int score = 0;
    time_t start_time = time(NULL);

    for (int i = 0; i < num_questions; i++) {
        printf("Question %d: %s", i + 1, questions[i].question);
        for (int j = 0; j < MAX_OPTIONS; j++) {
            printf("%s", questions[i].options[j]);
        }

        char answer;
        printf("Your answer: ");
        scanf(" %c", &answer);

        if (answer == questions[i].correct_option) {
            score++;
        }

        if (difftime(time(NULL), start_time) > time_limit) {
            printf("Time's up!\n");
            break;
        }
    }

    printf("Your score: %d out of %d\n", score, num_questions);
    save_record(username, score, num_questions);
}

int main() {
    Question questions[MAX_QUESTIONS];
    int num_questions;
    User users[MAX_USERS];
    int num_users;
    int logged_in_user = -1;

    load_questions(questions, &num_questions);
    shuffle_questions(questions, num_questions);
    load_users(users, &num_users);

    while (logged_in_user == -1) {
        int choice;
        printf("1. Register\n2. Login\nChoose an option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            register_user(users, &num_users);
        } else if (choice == 2) {
            logged_in_user = login_user(users, num_users);
        } else {
            printf("Invalid option. Try again.\n");
        }
    }

    int time_limit = 60; // 60 seconds for the exam
    printf("You have %d seconds to complete the exam.\n", time_limit);
    printf("Press Enter to start...\n");
    getchar(); // Wait for user to press Enter
    getchar(); // Capture the Enter key press

    conduct_exam(questions, num_questions, time_limit, users[logged_in_user].username);

    printf("Do you want to view your previous records? (y/n): ");
    char view_choice;
    scanf(" %c", &view_choice);
    if (view_choice == 'y' || view_choice == 'Y') {
        view_records(users[logged_in_user].username);
    }

    return 0;
}
