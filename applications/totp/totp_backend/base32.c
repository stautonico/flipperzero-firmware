#include "base32.h"

#include <furi.h>

bool totp_validate_base32_string(const char* str) {
    // This code was taken but modified from c_otp/lib/utils.c
    uint32_t len = strlen(str);
    if(((len & 0xF) != 0) && ((len & 0xF) != 8)) return false;

    for(int pos = 0; pos < len; pos++) {
        if(totp_base32_vals[str[pos]] == -1) return false;

        if(str[pos] == '=') {
            if(((pos & 0xF) == 0) || ((pos & 0xF) == 8)) return false;

            if((len - pos) > 6) return false;

            switch(pos % 8) {
            case 2:
            case 4:
            case 5:
            case 7:
                break;
            default:
                return false;
            }

            for(; (pos < len); pos++) {
                if(str[pos] != '=') return false;
            }
        }
    }

    return true;
}

uint32_t totp_decode_base32_string(uint8_t** k) {
    // This code was taken but modified (slightly) from c_otp/lib/utils.c

    uint32_t keylen = 0;
    uint32_t pos = 0;
    uint32_t len = strlen((char*)*k);

    for(; pos <= (len - 8); pos += 8) {
        // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
        // MB is middle bits             (0x7E == 01111110 ~= MB)
        // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)

        // byte 0
        (*k)[keylen + 0] = (totp_base32_vals[(*k)[pos + 0]] << 3) & 0xF8; // 5 MSB
        (*k)[keylen + 0] |= (totp_base32_vals[(*k)[pos + 1]] >> 2) & 0x07; // 3 LSB
        if((*k)[pos + 2] == '=') {
            keylen += 1;
            break;
        }

        // byte 1
        (*k)[keylen + 1] = (totp_base32_vals[(*k)[pos + 1]] << 6) & 0xC0; // 2 MSB
        (*k)[keylen + 1] |= (totp_base32_vals[(*k)[pos + 2]] << 1) & 0x3E; // 5  MB
        (*k)[keylen + 1] |= (totp_base32_vals[(*k)[pos + 3]] >> 4) & 0x01; // 1 LSB
        if((*k)[pos + 4] == '=') {
            keylen += 2;
            break;
        }

        // byte 2
        (*k)[keylen + 2] = (totp_base32_vals[(*k)[pos + 3]] << 4) & 0xF0; // 4 MSB
        (*k)[keylen + 2] |= (totp_base32_vals[(*k)[pos + 4]] >> 1) & 0x0F; // 4 LSB
        if((*k)[pos + 5] == '=') {
            keylen += 3;
            break;
        }

        // byte 3
        (*k)[keylen + 3] = (totp_base32_vals[(*k)[pos + 4]] << 7) & 0x80; // 1 MSB
        (*k)[keylen + 3] |= (totp_base32_vals[(*k)[pos + 5]] << 2) & 0x7C; // 5  MB
        (*k)[keylen + 3] |= (totp_base32_vals[(*k)[pos + 6]] >> 3) & 0x03; // 2 LSB
        if((*k)[pos + 7] == '=') {
            keylen += 4;
            break;
        }

        // byte 4
        (*k)[keylen + 4] = (totp_base32_vals[(*k)[pos + 6]] << 5) & 0xE0; // 3 MSB
        (*k)[keylen + 4] |= (totp_base32_vals[(*k)[pos + 7]] >> 0) & 0x1F; // 5 LSB
        keylen += 5;
    }

    (*k)[keylen] = 0; // null terminate the string

    return keylen;
}