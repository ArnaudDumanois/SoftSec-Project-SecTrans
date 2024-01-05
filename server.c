#include <stdio.h>
#include <unistd.h>
#include "constants.h"
#include "server.h"
#include "message_management/server_message_management/server_message_management.h"
#include "rsa/rsa_manager.h"
#include <string.h>
#include "aes/aes_manager.h"
#include "base64/base64encode.h"
#include "base64/base64decode.h"

int main() {
    // Démarrage du serveur
    //start_server(SERVER_PORT);
    //printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    //char message[INPUT_SIZE];
    RSA *keyPair = generate_RSA_keys();
    char *test = "Julien est un BG sans finiorjgriogjroigejgepkogrkeropkgrprkgrpoerkrpokaaaaaaaaaaaaaaaa";
    char *encrypted_msg;
    int res = encrypt_message(test, keyPair, &encrypted_msg);
    printf("res encrypt : %d\n", res);
    printf("Original Message: %s\n", test);
    printf("Encrypted Message: %s\n", encrypted_msg);

    char *decrypted_msg;
    decrypt_message(encrypted_msg, keyPair, &decrypted_msg);

    //printf("Decrypted Message: %s\n", decrypted_msg);

    // Free allocated memory
    /*RSA_free(keyPair);
    RSA_free(readPublicKey);
    free(publicKeyStr);
    free(encryptedMessage);
    free(decryptedMessage);*/


    /* EVP_CIPHER_CTX *en;
    EVP_CIPHER_CTX *de;
    unsigned char key[32];
    unsigned char iv[16];
    if (init_aes(&en, &de, key, iv) != 0) {
        printf("error init aes\n");
        return -1;
    }

    char *test = "Julien est un BG sans fin de la mort qui tue à l'infini Julien est un BG sans fin de la mort qui tue à l'infini Julien est un BG sans fin de la mort qui tue à l'infini";
    unsigned char *encrypted_msg;
    encrypt_aes(test, &encrypted_msg, en);
    printf("encrypted aes : %s\n", encrypted_msg);
    char *decrypted_msg;
    decrypt_aes(encrypted_msg, &decrypted_msg, strlen((char *) encrypted_msg), de);
    printf("decrypted aes : %s\n", decrypted_msg);*/
    /*listen_message(message);

    // Arrêt du serveur
    stop_server();
    printf("Serveur arrêté\n");*/

    /*char *test = "Julien est un BG sans finiorjgriogjroigejgepkogrkeropkgrprkgrpoerkrpokaaaaaaaaaaaaaaaa";
    printf("%ld\n", b64_encoded_size(strlen(test)));
    char * encoded = b64_encode((unsigned char *) test, strlen(test));
    size_t decoded_size = b64_decoded_size(encoded);
    printf("%ld\n", b64_decoded_size(encoded));
    unsigned char * out = malloc(decoded_size);
    b64_decode(encoded, out, decoded_size);
    printf("%s\n", encoded);
    printf("%s\n", out);*/


    return 0;
}

