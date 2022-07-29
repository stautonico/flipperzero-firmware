#include "rfc6238.h"

#include "rfc4226.h"

uint32_t totp_internal_TOTP(uint8_t* key, uint32_t keylen, uint64_t time, int digits) {
    uint32_t totp;

    totp = totp_internal_HOPT(key, keylen, time, digits);
    return totp;
}