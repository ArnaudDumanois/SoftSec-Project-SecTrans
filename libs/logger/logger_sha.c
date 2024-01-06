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
    int file_descriptor = open(USERS_DB_FILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    if (user_exists(usrname)) {
        printf("USER EXISTE !");
        close(file_descriptor);
        return -1;
    }

    User new_user;
    strcpy(new_user.username, usrname);
    generate_salt(new_user.salt);
    hash_password(passwd, new_user.salt, new_user.hashed_password);

    if (write(file_descriptor, &new_user, sizeof(User)) != sizeof(User)) {
        perror("Erreur lors de l'écriture de l'utilisateur dans le fichier");
        close(file_descriptor);
        return -1;
    }

    //Le compté est créé
    //on créé un dossier

    close(file_descriptor);
    return 1;
}

int authenticate_user(const char *username, const char *password) {
    int file_descriptor = open(USERS_DB_FILE, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    User user;
    ssize_t read_result;
    int authentication_result = 0;  // Par défaut, l'authentification échoue

    while ((read_result = read(file_descriptor, &user, sizeof(User))) == sizeof(User)) {
        printf("%s\n%s\n", user.username, user.hashed_password);
        if (strcmp(user.username, username) == 0) {
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_password(password, user.salt, hashed_password);
            printf("HASHED PASSWD: %s\n", hashed_password);

            if (strcmp(hashed_password, user.hashed_password) == 0) {
                authentication_result = 1; // Authentification réussie
            }

            break; // Utilisateur trouvé, que le mot de passe soit correct ou non
        }
    }

    if (read_result == -1) {
        perror("Erreur lors de la lecture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    close(file_descriptor);
    return authentication_result;
}

int user_exists(const char *username) {
    int file_descriptor = open(USERS_DB_FILE, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    User user;
    ssize_t read_result;
    int user_exists = 0;  // Par défaut, l'utilisateur n'existe pas

    while ((read_result = read(file_descriptor, &user, sizeof(User))) == sizeof(User)) {
        if (strcmp(user.username, username) == 0) {
            user_exists = 1;
            break;  // Utilisateur trouvé
        }
    }

    if (read_result == -1) {
        perror("Erreur lors de la lecture du fichier des utilisateurs");
        exit(EXIT_FAILURE);
    }

    close(file_descriptor);
    return user_exists;
}

