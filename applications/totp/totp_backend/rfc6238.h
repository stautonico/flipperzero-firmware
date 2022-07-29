#pragma once

#include <stdint.h>

uint32_t totp_internal_TOTP(uint8_t *key, uint32_t kl, uint64_t time, int digits);