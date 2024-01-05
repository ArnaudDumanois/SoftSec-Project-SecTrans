#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY_LENGTH  1024
#define PUB_EXP     3 // attention c'est ptet pas ouf ça

void generate_keys(char **private_key, char **public_key) {
    // Générer une nouvelle paire de clés RSA de 2048 bits
    RSA *keypair = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, PUB_EXP);

    if (RSA_generate_key_ex(keypair, KEY_LENGTH, e, NULL) != 1) {
        fprintf(stderr, "Erreur lors de la génération de la paire de clés RSA.\n");
        RSA_free(keypair);
        BN_free(e);
        EVP_cleanup();
        exit(EXIT_FAILURE);
    }

    // Utiliser les objets BIO pour stocker les clés
    BIO *pri_bio = BIO_new(BIO_s_mem());
    BIO *pub_bio = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri_bio, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub_bio, keypair);

    // Obtenir la longueur des clés
    size_t pri_len = BIO_pending(pri_bio);
    size_t pub_len = BIO_pending(pub_bio);

    // Allouer de la mémoire pour les clés
    *private_key = (char *)malloc(pri_len + 1);
    *public_key = (char *)malloc(pub_len + 1);

    // Lire les clés depuis les objets BIO
    BIO_read(pri_bio, *private_key, pri_len);
    BIO_read(pub_bio, *public_key, pub_len);

    // Ajouter un caractère nul à la fin des clés pour les rendre des chaînes C valides
    (*private_key)[pri_len] = '\0';
    (*public_key)[pub_len] = '\0';

    // Libérer la mémoire allouée pour la paire de clés
    RSA_free(keypair);
    BN_free(e);

    // Libérer les objets BIO
    BIO_free_all(pri_bio);
    BIO_free_all(pub_bio);
}