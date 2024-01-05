//
// Created by Julien on 03/01/2024.
//

#include "logger_sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 64
#define SALT_SIZE 16
#define USERS_DB_FILE "users.db"

void generate_salt(char *salt) {
    if (RAND_bytes((unsigned char*)salt, SALT_SIZE) != 1) {
        fprintf(stderr, "Erreur lors de la génération du sel.\n");
        exit(EXIT_FAILURE);
    }
}

void hash_password(const char *password, const char *salt, char *hashed_password) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salt, SALT_SIZE);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final((unsigned char *)hashed_password, &sha256);
}

void save_user(const User *user) {
    FILE *file = fopen(USERS_DB_FILE, "ab");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        exit(EXIT_FAILURE);
    }

    if (fwrite(user, sizeof(User), 1, file) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture de l'utilisateur dans le fichier.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(USERS_DB_FILE, "rb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        exit(EXIT_FAILURE);
    }

    User user;
    while (fread(&user, sizeof(User), 1, file) == 1) {
        if (strcmp(user.username, username) == 0) {
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_password(password, user.salt, hashed_password);
            if (strcmp(hashed_password, user.hashed_password) == 0) {
                fclose(file);
                return 1; // Authentification réussie
            } else {
                break; // Utilisateur trouvé, mais mot de passe incorrect
            }
        }
    }

    fclose(file);
    return 0; // Aucun utilisateur trouvé
}

int user_exists(const char *username, const char *password) {
    FILE *file = fopen(USERS_DB_FILE, "rb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        exit(EXIT_FAILURE);
    }

    User user;
    while (fread(&user, sizeof(User), 1, file) == 1) {
        if (strcmp(user.username, username) == 0) {
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_password(password, user.salt, hashed_password);
            if (strcmp(hashed_password, user.hashed_password) == 0) {
                fclose(file);
                return 1; // L'utilisateur existe et le mot de passe correspond
            } else {
                fclose(file);
                return 0; // L'utilisateur existe, mais le mot de passe ne correspond pas
            }
        }
    }
    fclose(file);
    return -1; // L'utilisateur n'existe pas
}

