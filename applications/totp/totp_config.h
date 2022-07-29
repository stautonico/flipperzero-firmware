#pragma once
#include "totp_app_i.h"

// Enter your timezone offset here
// Your timezone offset should be the difference between your local time and UTC time
// For example, new york (eastern daylight time) is 4 hours behind UTC. So you would enter 4.
// This feature will be removed and changed into a setting in the app eventually.
#define TOTP_TIMEZONE_OFFSET 4


#ifndef TOTP_MY_CODE_IS_BAD

// Enter the amount of keys here
#define TOTP_KEY_COUNT 9

// Add your totp keys here
// Insert name of key, base32 encoded secret, email (or username), duration, and key size
const TotpKeyEntry totp_keys[TOTP_KEY_COUNT] = {
    {"Google", "AABBCCDDEEFFGGHH", "test@gmail.com", 30, 6},
    {"Amazon", "IIJJKKLLMMNNOOPP", "another@gmail.com", 30, 6},
    {"Apple", "1122334455667788", "this@gmail.com", 30, 6},
    {"Microsoft", "MicrosoftKeyValu", "hax0r@email.org", 30, 6},
    {"Twitter", "TwitterKeyValuee", "hacker@gov.edu", 30, 6},
    {"TOTP Test Application", "XZDQUTZBWYELXEOOVDRETZ7VDDGUPF4P", "test@gmail.com", 30, 6},
    {"Bad Key", "INVALIDBASE32", "null", 30, 6},
    {"danhersam.com", "JBSWY3DPEHPK3PXP", "@testaccount", 30, 6},
    {"github.com", "PXK4BTRG5Z4B7YGK", "imsorrygithubineedthisfortesting", 30, 6}
};

#endif