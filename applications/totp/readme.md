# TOTP For Flipper

Before I say anything, I want to thank [Astra](https://github.com/Astrrra) for making this project possible.
They wrote all the actual TOTP code. I basically wrote a GUI wrapper around it. Thanks, Astra!

## Foreword

Before you install this, you should know that this is a **very** early version with many features missing.
This is not very secure at all, the TOTP keys are simply hardcoded into the program. This will be changed in the future.

**IMPORTANT NOTE: UNTIL THE TOTP CODES ARE STORED IN A FILE, MAKE SURE YOU DO NOT COMMIT YOUR TOTP SECRETS INTO YOUR GIT
REPOSITORY
IF YOU DO, YOU ARE POTENTIALLY COMPROMISING YOUR SECURITY, AND I AM NOT RESPONSIBLE FOR ANY COMPROMISED ACCOUNTS.
YOU HAVE BEEN WARNED**

Since this software is pretty beta-y, I would not recommend using it as your primary TOTP authentication method.
If you do find a bug, please report it to me (you can contact me via discord (@./steve.sh#0101) or email me @
stautonico@gmail.com)

## Installation

1. Copy the `lib/TOTP-MCU` directory to the `lib/` directory of your repo
2. Open the `lib/misc.scons` file in the text editor of your choice and add the following content:
    1. Add `"#/lib/TOTP-MCU"` to the `CPPPATH` section (quotes included)
    2. Add `"TOTP-MCU"` to the `libs_recurse` section (quotes included)
3. Copy the `applications/totp` directory to the `applications/` directory of your repo
4. Edit the `applications/totp/totp_config.h` and add the following content:
    1. Set the `TOTP_TIMEZONE_OFFSET` to the offset of your timezone from UTC. (More information
       in [Timezone Offset](#timezone-offset))
    2. Add some entries into the `totp_keys` array. (More information in [TOTP Keys](#totp-keys))
    3. Set the `TOTP_KEY_COUNT` to the number of TOTP keys you have.
5. Add `"totp"` into the `provides` array of the `basic_apps` section in `applications/meta/application.fam`
6. Compile as normal (`docker-compose up -d` & `docker-compose exec dev ./fbt`)
7. Flash to the flipper as normal (flash using qFlipper and select `dist/f7-C/flipper-z-f7-full-local.dfu` or use
   whatever other method you prefer)

## Timezone Offset

The TOTP algorithm uses the current time as the seed. This means both the server and the client (the flipper in this
case)
needs to have the same time. [The standard](https://datatracker.ietf.org/doc/html/rfc6238#section-3) states that the
TOTP
codes should be calculated using UTC as the timezone. The flipper's RTC (real-time clock) is synced with your computer's
local time whenever you connect it to qFlipper. To solve this, this TOTP app uses a timezone offset. This offset
is the difference between your local time and UTC (in hours)

For example, if you live in New York, your offset would be -4 (as New York is 4 hours behind UTC)<br>
If you live in London, your offset would be 1 (as London is 1 hour ahead of UTC)<br>

If you are unsure of your offset, you can use the [Timezone Offset Calculator](https://www.timeanddate.com/time/zone)
(enter your location into the bar and look at the "offset" table)

**I am not 100% sure of this, but I think you may have to adjust your timezone offset during daylight savings time (if
your location observes DST).** This is untested because daylight savings time hasn't happened at the time of writing
this app lol.

Eventually, you will be able to set your timezone from an in-app setting ([planned feature](#planned-features)).

## TOTP Keys

TOTP key entries are stored in the following format:

```
name - The name of the key, also called the "Issuer" (usually the name of the website, e.g. "github.com")
secret - The base32 encoded secret key (usually a 16 or 32 character string)
account - The username of the account (usually a username or email)
duration - The amount of time (in seconds) that the TOTP code is valid for. (default: 30) Note: This feature does nothing yet
code_size - The number of digits in the TOTP code. (default: 6) Note: This feature does nothing yet
```

## Planned Features

- [ ] Save TOTP keys to a file on disk (rather than hardcoded into the program)
- [ ] Add a settings menu that has the following settings:
    - [ ] Set the timezone offset
    - [ ] Add new keys (using the flipper's on-screen keyboard)
- [ ] Encrypt the TOTP keys file (using the flippers secure enclave)
- [ ] Add a custom icon for the app (instead of using the U2F icon)

## Credits

- [Astra](https://github.com/Astrrra) - Wrote all the important TOTP code
- [Fmount](https://github.com/fmount/c_otp) - Used as a reference for the TOTP code + borrowed
  the `totp_validate_base32_string` function
- Many people in the flipper discord for helping me learn how to write a flipper app!