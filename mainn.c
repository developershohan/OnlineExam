#include <stdio.h>
#include "user.h"
#include "question.h"
#include "admin.h"
#include "teacher.h"
void user_session(User *users, int num_users, Question *questions, int num_questions);

int main() {
    User users[MAX_USERS];
    Question questions[MAX_QUESTIONS];
    int num_users = 0;
    int num_questions = 0;

    // Load existing data
    load_users(users, &num_users);
    load_questions(questions, &num_questions);

    int choice, logged_in = 0;

    while (1) {
        printf("\n1. Login\n2. Register\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                logged_in = login_user(users, num_users);
                if (logged_in) {
                    // Assume a separate function handles post-login activities
                    user_session(users, num_users, questions, num_questions);
                }
                break;
            case 2:
                if (register_user(users, &num_users)) {
                    printf("Please login to continue.\n");
                }
                break;
            case 3:
                printf("Exiting program.\n");
                return 0;  // Exit the program
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
