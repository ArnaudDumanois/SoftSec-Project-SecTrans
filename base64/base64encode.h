#ifndef SOFTSEC_PROJECT_SECTRANS_BASE64ENCODE_H
#define SOFTSEC_PROJECT_SECTRANS_BASE64ENCODE_H

#include <stdlib.h>

size_t b64_encoded_size(size_t inlen);
char *b64_encode(const unsigned char *in, size_t len);

#endif //SOFTSEC_PROJECT_SECTRANS_BASE64ENCODE_H
