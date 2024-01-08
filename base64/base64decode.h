#ifndef SOFTSEC_PROJECT_SECTRANS_BASE64DECODE_H
#define SOFTSEC_PROJECT_SECTRANS_BASE64DECODE_H

#include <stdlib.h>

size_t b64_decoded_size(const char *in);
int b64_decode(const char *in, unsigned char *out, size_t outlen);

#endif //SOFTSEC_PROJECT_SECTRANS_BASE64DECODE_H
