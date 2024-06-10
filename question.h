#ifndef QUESTION_H
#define QUESTION_H

#include "common.h"

void load_questions(Question *questions, int *num_questions);
void save_questions(Question *questions, int num_questions);
void shuffle_questions(Question *questions, int num_questions);
void add_question(Question *questions, int *num_questions);
void edit_question(Question *questions, int num_questions);
void delete_question(Question *questions, int *num_questions);

#endif // QUESTION_H
