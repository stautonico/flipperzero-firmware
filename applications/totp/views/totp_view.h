#pragma once

#include <stdint.h>
#include <gui/view.h>

#include "../totp_app_i.h"

#include "../totp_helpers.h"

ViewTotp* view_totp_alloc();
void view_totp_free(ViewTotp* instance);

View* view_totp_get_view(ViewTotp* instance);