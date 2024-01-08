#include "../../common_message_management.h"
#include "../../../actions.h"
#include "../../../base64/base64encode.h"
#include "../client_message_management.h"
#include "../../../utils/array_utils.h"

void send_rsa_public_key(char msg_to_send[MESSAGE_SIZE], unsigned char *public_key) {
    add_action(msg_to_send, ACTION_PUB_KEY_SEND);
    add_string(msg_to_send, (char *) public_key);
    size_t encoded_size = b64_encoded_size(128 + 3);
    if (encoded_size >= 1024) {
        exit(EXIT_FAILURE);
    }
    char *encoded = b64_encode((unsigned char *) msg_to_send, encoded_size);

    sending(encoded, SERVER_PORT);
    clear_array(msg_to_send, MESSAGE_SIZE);
}