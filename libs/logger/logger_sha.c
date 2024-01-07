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
#include <sys/stat.h>
#include <dirent.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 64
#define SALT_SIZE 16
#define USERS_DB_FILE "users.db"

void createUserFolder(User *user);

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

    if (write(file_descriptor, new_user, sizeof(User))==-1) {
        perror("Erreur lors de l'écriture de l'utilisateur dans le fichier");
        close(file_descriptor);
        return INTERNAL_ERROR;
    }

    //Le compté est créé
    //on créé un dossier
    createUserFolder(new_user);

    close(file_descriptor);
    free(new_user);
    return REGISTRATION_DONE;
}

void createUserFolder(User *user) {
    char *folder_name = malloc(sizeof(user->username));
    strcat(folder_name, user->username);
    generate_salt(user->global_salt);
    char *folder_hashedname = malloc(sizeof(folder_name));
    hash_generator(folder_name, user->global_salt, folder_hashedname);
    printf("REGISTER FOLDER HASHED NAME : %s\n",folder_hashedname);

    struct stat st = {0};
    if (stat(ROOT_USERS_FOLDER, &st) == -1) {mkdir(ROOT_USERS_FOLDER, 0755);}
    char *final_path = malloc(sizeof(ROOT_USERS_FOLDER)+ sizeof(folder_hashedname));
    strcat(final_path,ROOT_USERS_FOLDER);
    strcat(final_path,"/");
    strcat(final_path,folder_hashedname);
    printf("FINAL PATH = %s\n",final_path);
    if (stat(final_path, &st) == -1) {mkdir(final_path, 0755);}
}

int authenticate_user(const char *username, const char *password) {
    printf("AUTH USER !\n");
    int file_descriptor = open(USERS_DB_FILE, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Erreur lors de l'ouverture du fichier des utilisateurs");
        return INTERNAL_ERROR;
    }

    User user;
    ssize_t read_result;
    int authentication_result = AUTH_ERROR;  // Par défaut, l'authentification échoue

    while ((read_result = read(file_descriptor, &user, sizeof(User)))) {
        if (compare(user.username, username)==TRUE) {
            printf("USERNAME SAME !\n");
            char hashed_password[MAX_PASSWORD_LENGTH];
            hash_generator(password, user.password_salt, hashed_password);

            printf("passwd_inside:\n%s\n",user.hashed_password);
            printf("generate passwd:\n%s\n",hashed_password);

            if(compare(hashed_password, user.hashed_password)==TRUE) {
                printf("AUTH DONE !\n");
                authentication_result = AUTH_DONE; // Authentification réussie
                char *folder_name = malloc(sizeof(user.username));
                strcat(folder_name, user.username);
                char *folder_hashedname = malloc(sizeof(user.username));
                hash_generator(folder_name, user.global_salt, folder_hashedname);
                printf("LOGIN FOLDER HASHED NAME : %s\n",folder_hashedname);
                char *final_user_path = malloc(sizeof(ROOT_USERS_FOLDER)+ sizeof(folder_hashedname));
                strcat(final_user_path,ROOT_USERS_FOLDER);
                strcat(final_user_path,"/");
                strcat(final_user_path,folder_hashedname);
                DIR *user_dir = opendir(final_user_path);
                struct dirent *dirp;
                while ((dirp = readdir(user_dir)) != NULL) printf("%s\n", dirp->d_name);
            }
            else{
                printf("AUTH ECHOUEE ! \n");
            }
        }
    }

    if(read_result==-1){

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

    while ((read_result = read(file_descriptor, &user, sizeof(User)))) {
        //print_user(&user);
        if (compare(user.username, username) == TRUE) {
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

//HYPOTESE : INPUT PARFAITES -> QUE DES charactères [0-9a-zA-Z]
boolean compare(const char a[],const char b[])
{
    if(strlen(a)!=strlen(b)){ printf("NOT SAME LENGTH !\n");return FALSE;}
    boolean flag=TRUE,i=0;
    while(a[i]!='\0' &&b[i]!='\0')
    {
        if(a[i]!=b[i]){
            flag=FALSE;
            break;
        }
        i++;
    }
    printf("SAME COMPARAISON ?: %d\n",flag);
    return flag;
}

void print_user(User *user){
    printf("USER : %s\n",user->username);
    printf("PASSWD : %s\n\n",user->hashed_password);
}

