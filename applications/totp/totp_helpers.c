#include "totp_helpers.h"

#include "totp_app_i.h"

#include "totp_app.h"
#include "totp_view.h"

#include <gui/modules/variable_item_list.h>

#define TOTP_KEY_COUNT 8

// Add your totp keys here
const TotpKeyEntry totp_keys[TOTP_KEY_COUNT] = {
    {"Google", "AABBCCDDEEFFGGHH", "test@gmail.com"},
    {"Amazon", "IIJJKKLLMMNNOOPP", "another@gmail.com"},
    {"Apple", "1122334455667788", "this@gmail.com"},
    {"Microsoft", "MicrosoftKeyValu", "hax0r@email.org"},
    {"Twitter", "TwitterKeyValuee", "hacker@gov.edu"},
    {"TOTP Test Application", "XZDQUTZBWYELXEOOVDRETZ7VDDGUPF4P", "test@gmail.com"},
    {"Bad Key", "INVALIDBASE32", "null"},
    {"danhersam.com", "JBSWY3DPEHPK3PXP", "@testaccount"}
};

static void totp_select_key_callback(void* context, uint32_t index) {
    FURI_LOG_D("totp", "totp_select_key_callback::Starting", index);
    TotpApp* app = context;
    UNUSED(app);
    // Save our current state and switch views
    FURI_LOG_D("totp", "totp_select_key_callback::Selecting key %d", index);

    FURI_LOG_D("totp", "totp_select_key_callback::Setting up state");
    with_view_model(
        view_totp_get_view(app->view_totp), (ViewTotpModel * model) {
            model->entry.name = totp_keys[index].name;
            model->entry.secret = totp_keys[index].secret;
            model->entry.account = totp_keys[index].account;

            return NULL;
        });
    FURI_LOG_D("totp", "totp_select_key_callback::Set up state!");

    FURI_LOG_D("totp", "totp_select_key_callback::Switching to key view");
    view_dispatcher_switch_to_view(app->view_dispatcher, TotpViewKey);
}

void totp_make_key_list(TotpApp* app) {
    VariableItem* item;
    // Initialize state?

    for(int i = 0; i < TOTP_KEY_COUNT; i++) {
        submenu_add_item(app->submenu, totp_keys[i].name, i, totp_select_key_callback, app);

        //        item = variable_item_list_add(
        //            app->variable_item_list,
        //            keys[i].name,
        //            1,
        //            totp_select_key_callback,
        //            app
        //        );
    }
}