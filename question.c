#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "question.h"

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

void save_questions(Question *questions, int num_questions) {
    FILE *file = fopen("questions.txt", "w");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_questions; i++) {
        fprintf(file, "%s", questions[i].question);
        for (int j = 0; j < MAX_OPTIONS; j++) {
            fprintf(file, "%s", questions[i].options[j]);
        }
        fprintf(file, "%c\n", questions[i].correct_option);
    }

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

void add_question(Question *questions, int *num_questions) {
    if (*num_questions >= MAX_QUESTIONS) {
        printf("Question bank is full.\n");
        return;
    }

    printf("Enter the question: ");
    getchar(); // Clear newline character from previous input
    fgets(questions[*num_questions].question, MAX_LINE_LENGTH, stdin);

    for (int i = 0; i < MAX_OPTIONS; i++) {
        printf("Enter option %c: ", 'A' + i);
        fgets(questions[*num_questions].options[i], MAX_LINE_LENGTH, stdin);
    }

    printf("Enter the correct option (A-D): ");
    scanf(" %c", &questions[*num_questions].correct_option);

    (*num_questions)++;
    save_questions(questions, *num_questions);
    printf("Question added successfully.\n");
}

void edit_question(Question *questions, int num_questions) {
    int question_number;

    printf("Enter the question number to edit (1-%d): ", num_questions);
    scanf("%d", &question_number);

    if (question_number < 1 || question_number > num_questions) {
        printf("Invalid question number.\n");
        return;
    }

    question_number--; // Adjust for 0-based index

    printf("Enter the new question: ");
    getchar(); // Clear newline character from previous input
    fgets(questions[question_number].question, MAX_LINE_LENGTH, stdin);

    for (int i = 0; i < MAX_OPTIONS; i++) {
        printf("Enter new option %c: ", 'A' + i);
        fgets(questions[question_number].options[i], MAX_LINE_LENGTH, stdin);
    }

    printf("Enter the new correct option (A-D): ");
    scanf(" %c", &questions[question_number].correct_option);

    save_questions(questions, num_questions);
    printf("Question edited successfully.\n");
}

void delete_question(Question *questions, int *num_questions) {
    int question_number;

    printf("Enter the question number to delete (1-%d): ", *num_questions);
    scanf("%d", &question_number);

    if (question_number < 1 || question_number > *num_questions) {
        printf("Invalid question number.\n");
        return;
    }

    question_number--; // Adjust for 0-based index

    for (int i = question_number; i < *num_questions - 1; i++) {
        questions[i] = questions[i + 1];
    }

    (*num_questions)--;
    save_questions(questions, *num_questions);
    printf("Question deleted successfully.\n");
}
