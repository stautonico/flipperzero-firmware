#include "totp_backend.h"

#include "base32.h"
#include <furi.h>
#include <furi_hal.h>
#include <math.h>
#include <time.h>

#include "rfc6238.h"

#define T0 0 // Idk why many totp implementations bother defining this



uint32_t totp_generate_totp_code(uint8_t *k, uint32_t keylen, uint32_t step, uint32_t digits) {
    // Some of this code was taken from c_otp/main.c, but modified to fit our needs.

    // Get the current time
    FuriHalRtcDateTime datetime;
    furi_hal_rtc_get_datetime(&datetime);
    uint64_t timestamp = furi_hal_rtc_datetime_to_timestamp(&datetime);

    FURI_LOG_I("totp", "Our current timestamp is %d", timestamp);

    // I don't understand why we bother subtracting T0 from the timestamp
    // but many implementations do it, so we do too.
    uint64_t time = floor((timestamp - T0) / step);

    return totp_internal_TOTP(k, keylen, time, digits);
}

TotpCalculationResult totp_generate_totp_NEW(const char* secret, uint32_t step, uint32_t digits) {
    // Steps
    //  1. Validate that our secret is a valid base32 string.
    //  2. Decode the secret into a byte array.
    //  3. Generate the TOTP code.
    //  4. Return the code and the time remaining until the next code is valid.
    // Some of this code was taken from c_otp/main.c, but modified to fit our needs.

    // 1. Validate that our secret is a valid base32 string.
    if(!totp_validate_base32_string(secret)) {
        FURI_LOG_E(
            "totp",
            "totp_generate_totp::The secret \"%s\" is not a valid base32 secret key!",
            secret);
        return (TotpCalculationResult){.code = -1, .expires_in = -1};
    }

    // 2. Decode the secret into a byte array.
    uint8_t *k = (uint8_t *) secret;
    uint32_t keylen = totp_decode_base32_string(&k);

    // 3. Generate the TOTP code.
    uint32_t code = totp_generate_totp_code(k, keylen, step, digits);

    // Maybe avoid getting the time again?
    FuriHalRtcDateTime datetime = {0};
    furi_hal_rtc_get_datetime(&datetime);

    struct tm date = {0};
    date.tm_hour = datetime.hour;
    date.tm_min = datetime.minute;
    date.tm_sec = datetime.second;
    date.tm_mday = datetime.day;
    date.tm_mon = datetime.month;
    date.tm_year = datetime.year;

    // 4. Return the code and the time remaining until the next code is valid.
    return (TotpCalculationResult){.code = code, .expires_in = 30 - (date.tm_sec % 30)};
}
