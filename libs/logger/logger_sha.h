//
// Created by Julien on 03/01/2024.
//

#ifndef SOFTSEC_PROJECT_SECTRANS_LOGGER_SHA_H
#define SOFTSEC_PROJECT_SECTRANS_LOGGER_SHA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 64
#define SALT_SIZE 16
#define USERS_DB_FILE "users.db"

#define INTERNAL_ERROR (-1)
#define USER_ALREADY_EXIST (-2)
#define ERROR_USERNAME_TOO_LONG (-3)
#define ERROR_PASSWORD_TOO_LONG (-4)
#define AUTH_ERROR (-5)
#define USER_NOT_EXIST (-7)

#define AUTH_DONE 0
#define REGISTRATION_DONE 1




typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password_salt[SALT_SIZE];
    char hashed_password[MAX_PASSWORD_LENGTH];
    char global_salt[SALT_SIZE];
} User;

void generate_salt(char *salt);
void hash_generator(const char *input, const char *salt, char *hashed_output);
int save_user(const char* usrname,const char* passwd);
int authenticate_user(const char *username, const char *password);
int user_exists(const char *username);
int compare(const char a[],const char b[]);
void print_user(User *user);

#endif //SOFTSEC_PROJECT_SECTRANS_LOGGER_SHA_H
