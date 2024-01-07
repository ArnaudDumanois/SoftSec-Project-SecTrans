#include <stdio.h>
#include "constants.h"
#include "server.h"
#include "message_management/server_message_management/server_message_management.h"
#include "rsa/rsa_manager.h"
#include <string.h>

int main() {
    // Démarrage du serveur
    start_server(SERVER_PORT);
    printf("Serveur démarré sur le port %d\n", SERVER_PORT);

    // Traitement des connexions (exemple : attente d'un message)
    char message[INPUT_SIZE];
    RSA *keyPair = generate_RSA_keys();
    char *test = "Julien est un BG sans fin";
    char *encrypted_msg;
    int res = encrypt_message(test, keyPair, &encrypted_msg);
    printf("res encrypt : %d\n", res);
    printf("Original Message: %s\n", test);
    printf("Encrypted Message: %s\n", encrypted_msg);

    char *decrypted_msg;
    decrypt_message(encrypted_msg, keyPair, &decrypted_msg);

    printf("Decrypted Message: %s\n", decrypted_msg);

    // Free allocated memory
    /*RSA_free(keyPair);
    RSA_free(readPublicKey);
    free(publicKeyStr);
    free(encryptedMessage);
    free(decryptedMessage);*/
    listen_message(message);

    // Arrêt du serveur
    stop_server();
    printf("Serveur arrêté\n");

    return 0;
}

