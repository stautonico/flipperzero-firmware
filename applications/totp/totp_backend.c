#include "totp_backend.h"

#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <input/input.h>

#include <notification/notification_messages.h>

#include <TOTP.h>

#include <base32.h>
#include <time.h>

#include <stdbool.h>

#include <toolbox/path.h>
#include <flipper_format/flipper_format.h>

#define TOTP_MY_CODE_IS_BAD
#include "totp_config.h"

#define KEY_LENGTH 10
#define TIME_STEP 30

// Taken from c_otp/lib/utils.c
static const int8_t totp_base32_vals[256] = {
    //    This map cheats and interprets:
    //       - the numeral zero as the letter "O" as in oscar
    //       - the numeral one as the letter "L" as in lima
    //       - the numeral eight as the letter "B" as in bravo
    // 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    14, 11, 26, 27, 28, 29, 30, 31, 1,  -1, -1, -1, -1, 0,  -1, -1, // 0x30
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // 0x40
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // 0x60
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, -1, -1, -1, -1, // 0x70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};

const TotpCalculationResult TOTP_CALCULATION_RESULT_INVALID_SECRET = {
    .code = NULL,
    .expires_in = -1};

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

TotpCalculationResult totp_generate_totp(const unsigned char* secret) {
    // Validate our secret before we continue
    if(!totp_validate_base32_string((char*)secret)) {
        return TOTP_CALCULATION_RESULT_INVALID_SECRET;
    }

    uint8_t hmacKey[20];

    base32_decode(secret, hmacKey, KEY_LENGTH);

    TOTP(hmacKey, KEY_LENGTH, TIME_STEP); // Secret key, Secret key length, Timestep (30s)

    FuriHalRtcDateTime datetime = {0};
    furi_hal_rtc_get_datetime(&datetime);

    struct tm date = {0};
    // We subtract the timezone offset instead of adding it so the user can just enter their
    // difference between UTC and their local timezone.
    // Example: New York is -4 hours from UTC, so the user enters -4.
    date.tm_hour = datetime.hour - TOTP_TIMEZONE_OFFSET;
    date.tm_min = datetime.minute;
    date.tm_sec = datetime.second;
    date.tm_mday = datetime.day;
    date.tm_mon = datetime.month - 1;
    date.tm_year = datetime.year - 1900;
    // god i hate these 5 lines

    uint32_t newCode = getCodeFromTimestamp(mktime(&date));

    FURI_LOG_D("totp", "totp_generate_totp::Our new code is %d", newCode);

    // 6 + \0
    char* code_string = malloc(sizeof(char) * 7);
    snprintf(code_string, sizeof(char) * 7, "%06ld", newCode);

    FURI_LOG_D("totp", "totp_generate_totp::Our sprintfed code is %s", code_string);

    TotpCalculationResult result = {code_string, (29 - (date.tm_sec % 30)) + 1};

    return result;
}
