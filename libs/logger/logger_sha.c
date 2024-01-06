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
    if (RAND_bytes((unsigned char *) salt, SALT_SIZE) != 1) {
        fprintf(stderr, "Erreur lors de la génération du sel.\n");
        exit(EXIT_FAILURE);
    }
}

void hash_generator(const char *input, const char *salt, char *hashed_output) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salt, SALT_SIZE);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final((unsigned char *) hashed_output, &sha256);
}

int save_user(const char *usrname, const char *passwd) {
    int file_descriptor = open(USERS_DB_FILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        close(file_descriptor);
        return INTERNAL_ERROR;
    }

    if (user_exists(usrname) == USER_ALREADY_EXIST) {
        printf("USER EXISTE !");
        close(file_descriptor);
        return USER_ALREADY_EXIST;
    }

    User *new_user = malloc(sizeof(User));
    strcpy(new_user->username, usrname);
    generate_salt(new_user->password_salt);
    hash_generator(passwd, new_user->password_salt, new_user->hashed_password);

    if (write(file_descriptor, new_user, sizeof(User)) != sizeof(User)) { <- ça c'est carrément foireux
        perror("Erreur lors de l'écriture de l'utilisateur dans le fichier");
        close(file_descriptor);
        return INTERNAL_ERROR;
    }

    printf("passwd salt: %s\n", new_user->password_salt);

    //Le compté est créé
    //on créé un dossier
    char *folder_name = malloc(sizeof(new_user->username) + sizeof(new_user->hashed_password));
    strcat(folder_name, new_user->username);
    strcat(folder_name, new_user->hashed_password);
    generate_salt(new_user->global_salt);
    char *folder_hashedname = malloc(sizeof(folder_name));
    hash_generator(folder_name, new_user->global_salt, folder_hashedname);
    //printf("REGISTER FOLDER HASHED NAME : %s\n",folder_hashedname);
    close(file_descriptor);
    free(new_user);
    return REGISTRATION_DONE;
}

int authenticate_user(const char *username, const char *password) {
    int file_descriptor = open(USERS_DB_FILE, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        return INTERNAL_ERROR;
    }

    User user;
    ssize_t read_result;
    int authentication_result = AUTH_ERROR;  // Par défaut, l'authentification échoue

    while ((read_result = read(file_descriptor, &user, sizeof(User))) == sizeof(User)) {
        if (strcmp(user.username, username) == 0) {
            printf("USERNAME SAME !\n");
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_generator(password, user.password_salt, hashed_password);

            printf("passwd salt:\n\n");
            printf("%s", user.password_salt);

            if (strcmp(hashed_password, user.hashed_password) == 0) {
                authentication_result = AUTH_DONE; // Authentification réussie

                char *folder_name = malloc(sizeof(user.username) + sizeof(user.hashed_password));
                strcat(folder_name, user.username);
                strcat(folder_name, user.hashed_password);
                generate_salt(user.global_salt);
                char *folder_hashedname = malloc(sizeof(user.username) + sizeof(user.hashed_password));
                hash_generator(folder_name, user.global_salt, folder_hashedname);
                //printf("LOGIN FOLDER HASHED NAME : %s\n",folder_hashedname);
            }

            break; // Utilisateur trouvé, que le mot de passe soit correct ou non
        }
    }

    if (read_result == -1) {
        perror("Erreur lors de la lecture du fichier des utilisateurs");
        return INTERNAL_ERROR;
    }

    close(file_descriptor);
    return authentication_result;
}

int user_exists(const char *username) {
    int file_descriptor = open(USERS_DB_FILE, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        return INTERNAL_ERROR;
    }

    User user;
    ssize_t read_result;
    int result = USER_NOT_EXIST;  // Par défaut, l'utilisateur n'existe pas

    while ((read_result = read(file_descriptor, &user, sizeof(User))) == sizeof(User)) {
        if (strcmp(user.username, username) == 0) {
            result = USER_ALREADY_EXIST;
            break;  // Utilisateur trouvé
        }
    }

    if (read_result == -1) {
        perror("Erreur lors de la lecture du fichier des utilisateurs");
        return INTERNAL_ERROR;
    }

    close(file_descriptor);
    return result;
}

