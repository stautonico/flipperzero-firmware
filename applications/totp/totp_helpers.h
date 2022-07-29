#pragma once

#include "totp_app_i.h"

#include "totp_app.h"

void totp_make_key_list(TotpApp* app);

typedef struct TotpKeyEntry TotpKeyEntry;