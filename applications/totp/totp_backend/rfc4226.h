#pragma once

#include <stdint.h>

uint8_t *totp_hmac(uint8_t *key, int keylen, uint64_t interval);


uint32_t totp_internal_HOPT(uint8_t *key, uint32_t keylen, uint64_t interval, int digits);


uint32_t totp_internal_DT(uint8_t *digest);