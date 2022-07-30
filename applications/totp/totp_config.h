#pragma once
#include "totp_app_i.h"

// Enter your timezone offset here
// Your timezone offset should be the difference between your local time and UTC time
// For example, new york (eastern daylight time) is 4 hours behind UTC. So you would enter 4.
// This feature will be removed and changed into a setting in the app eventually.
#define TOTP_TIMEZONE_OFFSET -4


// Ignore this, without it, we end up with multiple definitions of `totp_keys` even though we
// have pragma once. Include guards also doesn't fix the problem. I'll look more into it later.
#ifndef TOTP_MY_CODE_IS_BAD

// Enter the amount of keys here
#define TOTP_KEY_COUNT 2

// Add your totp keys here
// Insert name of key, base32 encoded secret, email (or username), duration, and key size
const TotpKeyEntry totp_keys[TOTP_KEY_COUNT] = {
    {"My Key", "MYENCODEDSECRETS", "test@gmail.com", 30, 6},
    {"My Key 2", "AABBCCDDEEFFGGHH", "my_username", 30, 6}
};

#endif