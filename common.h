#ifndef COMMON_H
#define COMMON_H

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

typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} Teacher;

typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} Admin;

#endif // COMMON_H
