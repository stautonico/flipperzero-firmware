#include "totp_backend.h"

//#include <furi.h>
//#include <furi_hal.h>
//#include <base32.h>
//#include <time.h>
//#include <totp.h>


#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <input/input.h>

#include <notification/notification_messages.h>

#include <TOTP.h>

#include <base32.h>
#include <time.h>

#include <toolbox/path.h>
#include <flipper_format/flipper_format.h>

#define KEY_LENGTH 6
#define TIME_STEP 30

TotpCalculationResult totp_generate_totp(const char* secret) {
    uint8_t hmacKey[20];

    int timezone = -3;

    base32_decode(secret, hmacKey, KEY_LENGTH);

    TOTP(hmacKey, KEY_LENGTH, TIME_STEP); // Secret key, Secret key length, Timestep (30s)

    FuriHalRtcDateTime datetime = {0};
    furi_hal_rtc_get_datetime(&datetime);

    struct tm date = {0};
//    date.tm_hour = datetime.hour + timezone;
    date.tm_hour = datetime.hour;
    date.tm_min = datetime.minute;
    date.tm_sec = datetime.second;
    date.tm_mday = datetime.day;
//    date.tm_mon = datetime.month - 1;
    date.tm_mon = datetime.month;
//    date.tm_year = datetime.year - 1900;
    date.tm_year = datetime.year;
    // god i hate these 5 lines

    uint32_t newCode = getCodeFromTimestamp(mktime(&date));

    FURI_LOG_D("totp", "totp_generate_totp::Our new code is %d", newCode);

    char* code_string = malloc(sizeof(char) * 6);
    sprintf(code_string, "%06ld", newCode);

    FURI_LOG_D("totp", "totp_generate_totp::Our sprintfed code is %s", code_string);

    TotpCalculationResult result = {code_string, (29 - (date.tm_sec % 30))};


    return result;
}