#include "totp_app.h"

#include "totp_app_i.h"

#include "totp_helpers.h"

static void totp_start_menu_callback(void* context, uint32_t index) {
    // Do something when pressing one of the keys on the start menu
}

// Navigation callbacks
static uint32_t totp_previous_callback(void* context) {
    // Return to the start menu from the key menu
    UNUSED(context);
    return TotpViewStart;
}

static uint32_t totp_exit_callback(void* context) {
    // Exit the app if we hit back from the start menu
    UNUSED(context);
    return VIEW_NONE;
}

TotpApp* totp_app_alloc() {
    FURI_LOG_D("totp", "totp_app_alloc::Allocating app");
    TotpApp* app = malloc(sizeof(TotpApp));

    View* view = NULL;

    // Initialize/allocate basic graphics items
    FURI_LOG_D("totp", "totp_app_alloc::Setting up GUI and view dispatcher");
    app->gui = furi_record_open("gui");
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Setup our views
    FURI_LOG_D("totp", "totp_app_alloc::Allocating view");
    app->view_totp = view_totp_alloc();
    view = view_totp_get_view(app->view_totp);
    view_set_previous_callback(view, totp_previous_callback);
    view_dispatcher_add_view(app->view_dispatcher, TotpViewKey, view);

    FURI_LOG_D("totp", "totp_app_alloc::Setting up VIL");
    // Set up our variable list for the start menu
    //    app->variable_item_list = variable_item_list_alloc();
    //    view = variable_item_list_get_view(app->variable_item_list);
    //    view_set_previous_callback(view, totp_exit_callback);
    //    view_dispatcher_add_view(app->view_dispatcher, TotpViewStart, view);

    // Setup our "submenu" (start menu)
    app->submenu = submenu_alloc();
    view = submenu_get_view(app->submenu);
    view_set_previous_callback(view, totp_exit_callback);
    view_dispatcher_add_view(app->view_dispatcher, TotpViewStart, view);

    // Make our list of keys
    FURI_LOG_D("totp", "totp_app_alloc::Making our key list");
    totp_make_key_list(app);

    FURI_LOG_D("totp", "totp_app_alloc::Done allocating!");
    return app;
}

void totp_app_free(TotpApp* app) {
    // Remove our views
    view_dispatcher_remove_view(app->view_dispatcher, TotpViewStart);
    view_dispatcher_remove_view(app->view_dispatcher, TotpViewKey);

    //    variable_item_list_free(app->variable_item_list);

    view_totp_free(app->view_totp);

    // Free our graphics items
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close("gui");

    free(app);
}

int32_t totp_app_run(TotpApp* app) {
    view_dispatcher_switch_to_view(app->view_dispatcher, TotpViewStart);
    view_dispatcher_run(app->view_dispatcher);

    return 0;
}

int32_t totp_app(void* p) {
    UNUSED(p);

    FURI_LOG_D("totp", "Starting totp app...");
    FURI_LOG_D("totp", "Allocating...");
    TotpApp* app = totp_app_alloc();
    FURI_LOG_D("totp", "Done!");

    FURI_LOG_D("totp", "Starting to run...");
    int32_t status = totp_app_run(app);

    FURI_LOG_D("totp", "Freeing...");
    totp_app_free(app);
    FURI_LOG_D("totp", "Done!");

    FURI_LOG_D("totp", "Exiting...");
    return status;
}