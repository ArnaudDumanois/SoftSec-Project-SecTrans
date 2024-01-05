//
// Created by Julien on 03/01/2024.
//

#include "logger_sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <unistd.h>
#include <fcntl.h>

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

int save_user(const char *usrname, const char *passwd) {
    FILE *file = fopen(USERS_DB_FILE, "ab");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des utilisateurs.\n");
        exit(EXIT_FAILURE);
    }

    if(user_exists(usrname,passwd)!=-1){
        printf("USER EXISTE !");
        return -1;
    }

    User *new_user = (User *)malloc(sizeof(User));
    strcpy(new_user->username,usrname);
    generate_salt(new_user->salt);
    hash_password(passwd, new_user->salt, new_user->hashed_password);

    if (fwrite(new_user, sizeof(User), 1, file) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture de l'utilisateur dans le fichier.\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 1;
}

int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(USERS_DB_FILE, "ab");

    if (!file) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    User user;
    size_t read_result;
    int authentication_result = 0;  // Par défaut, l'authentification échoue

    while ((read_result = fread(&user, sizeof(User), 1, file)) == 1) {
        if (strcmp(user.username, username) == 0) {
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_password(password, user.salt, hashed_password);

            if (strcmp(hashed_password, user.hashed_password) == 0) {
                authentication_result = 1; // Authentification réussie
            }
            break; // Utilisateur trouvé, que le mot de passe soit correct ou non
        }
    }

    if (read_result == 0 && !feof(file)) {
        perror("Erreur lors de la lecture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return authentication_result;
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

