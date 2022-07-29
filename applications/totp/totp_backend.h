#pragma once

#include <stdint.h>


typedef struct TotpCalculationResult {
    char* code;
    int expires_in;
} TotpCalculationResult;

TotpCalculationResult totp_generate_totp(const unsigned char* secret);