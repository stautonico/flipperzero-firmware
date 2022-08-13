#include "totp_app.h"

#include "totp_app_i.h"

#include "totp_helpers.h"

#include <storage/storage.h>
#include <flipper_format/flipper_format.h>
#include <stdbool.h>

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

// File functions
void totp_init_keys_file(TotpApp* app) {
    FURI_LOG_D("totp", "totp_load_keys::Loading keys");
    Storage* storage = furi_record_open("storage");
    FlipperFormat* file = flipper_format_file_alloc(storage);

    do {
        // Make the directory (if not exists)
        if(!storage_simply_mkdir(storage, TOTP_APP_FOLDER)) break;

        // Open the file and write the header
        if(!flipper_format_file_open_always(file, "/ext/totp/keys.totp")) break;
        if(!flipper_format_write_header_cstr(file, TOTP_FILE_HEADER, TOTP_FILE_VERSION)) break;
    } while(0);

    flipper_format_free(file);
    furi_record_close("storage");
}

bool totp_load_keys(TotpApp* app) {
    // Open the file

    Storage* storage = furi_record_open("storage");

    File* key_file = storage_file_alloc(storage);

    uint8_t path_len = strlen(TOTP_APP_FOLDER) + strlen("keys") + strlen(TOTP_APP_EXTENSION) + 2;
    char* key_file_path = malloc(sizeof(char) * path_len);
    snprintf(key_file_path, path_len, "%s/keys%s", TOTP_APP_FOLDER, TOTP_APP_EXTENSION);
    FURI_LOG_D("totp", "totp_load_keys::Loading keys from %s", key_file_path);

    bool is_file_opened =
        storage_file_open(key_file, key_file_path, FSAM_READ, FSOM_OPEN_EXISTING);

    FURI_LOG_D("totp", "totp_load_keys::Was file opened? %d", is_file_opened);

    // Free before we forget
    free(key_file_path);

    if(!is_file_opened) {
        FURI_LOG_D("totp", "totp_load_keys::Cannot open key file");
        storage_file_close(key_file);
        storage_file_free(key_file);
        furi_record_close("storage");
        return false;
    }

    FURI_LOG_D("totp", "totp_load_keys::File opened, starting to allocate and read");

    // Try to read the content from the file
    // Read one byte at a time until we hit a newline
    // If we hit a newline, make sure we don't have: the header line, the version line, or a line that starts with '#' (comment)
    // If the first two lines are not the header and version line, then we have an invalid file
    // Once we hit a valid newline, we can parse the line and add it to the list of keys
    // If we hit EOF, we are done
    FURI_LOG_D("totp", "totp_load_keys::Allocating buffer");
    const uint8_t buffer_size = 256;
    char* buf = malloc(sizeof(char) * buffer_size);

    bool ignore_line = false;
    uint32_t current_line = 0;
    FURI_LOG_D("totp", "totp_load_keys::Allocating key list");
    // Create a dynamic array of keys
    // We can't use a fixed size array because we don't know how many keys we have
//    char* key_lines = malloc(sizeof(char) * 128);

    while(1) {
        FURI_LOG_D("totp", "totp_load_keys::Reading line %d", current_line);
        current_line++;
        if (current_line > 100) {
            FURI_LOG_D("totp", "totp_load_keys::Too many lines");
            break;
        }
        uint16_t bytes_read = storage_file_read(key_file, buf, buffer_size);
        FURI_LOG_D("totp", "totp_load_keys::Read %d bytes", bytes_read);

//        if(bytes_read == 0) {
//            break;
//        }
        //
        //        //        for (uint8_t i; i < sizeof(buf); i++) {
        //        //            if(buf[i] == '\n') {
        //        //                ignore_line = false;
        //        //            }
        //        //
        //        //            if(buf[i] == '#') {
        //        //                ignore_line = true;
        //        //            }
        //        //
        //        //            if (!ignore_line) {
        //        //                if (buf[i] == '\n') {
        //        //                    current_line++;
        //        //                    key_lines[current_line] += '\0';
        //        //                    key_lines = realloc(key_lines, (sizeof(char) * 128) * (current_line + 1));
        //        //                } else {
        //        //                    key_lines[current_line] += buf;
        //        //                }
        //        //            }
        //        //        }
        // Do something with the data?
        //        FURI_LOG_D("totp", "totp_load_keys::%s", buf);
    }
    //
    //    for(uint32_t i = 0; i < current_line + 1; i++) {
    ////        FURI_LOG_D("totp", "totp_load_keys::%s", key_lines[i]);
    //    }

    free(buf);

    storage_file_close(key_file);
    storage_file_free(key_file);
    furi_record_close("storage");

    return true;
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
    view_dispatcher_add_view(app->view_dispatcher, TotpViewAbout, view);

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

    FURI_LOG_D("totp", "Loading keys from disk...");
    //    totp_init_keys_file(app);
    totp_load_keys(app);

    FURI_LOG_D("totp", "Starting to run...");
    int32_t status = totp_app_run(app);

    FURI_LOG_D("totp", "Freeing...");
    totp_app_free(app);
    FURI_LOG_D("totp", "Done!");

    FURI_LOG_D("totp", "Exiting...");
    return status;
}