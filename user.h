#ifndef USER_H
#define USER_H

#include "common.h"

void load_users(User *users, int *num_users);
int register_user(User *users, int *num_users);
int login_user(User *users, int num_users);

#endif // USER_H
