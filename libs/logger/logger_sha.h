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

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char salt[SALT_SIZE];
    char hashed_password[MAX_PASSWORD_LENGTH];
} User;

void generate_salt(char *salt);
void hash_password(const char *password, const char *salt, char *hashed_password);
int save_user(const char* usrname,const char* passwd);
int authenticate_user(const char *username, const char *password);
int user_exists(const char *username, const char *password);

#endif //SOFTSEC_PROJECT_SECTRANS_LOGGER_SHA_H
