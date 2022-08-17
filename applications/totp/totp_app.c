#include "totp_app.h"

#include "totp_app_i.h"

#include "totp_helpers.h"

#include <storage/storage.h>
#include <flipper_format/flipper_format.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
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
    size_t buffer_size = 1024;
    char* buf = malloc(sizeof(char) * buffer_size);
    bool first_alloc = true;

    while(1) {
        uint16_t bytes_read = storage_file_read(key_file, buf, buffer_size);
        FURI_LOG_D("totp", "totp_load_keys::Read %d bytes", bytes_read);

        if(bytes_read == 0) {
            break;
        }

        if(first_alloc) {
            buf = realloc(buf, sizeof(char) * (bytes_read));
            first_alloc = false;
            buffer_size = bytes_read;
        } else {
            buffer_size += bytes_read;
            buf = realloc(buf, sizeof(char) * buffer_size);
        }
    }

    char** split_lines = str_split(buf, '\n');

    // Print out the lines (for debugging)
    for(int i = 0; split_lines[i] != NULL; i++) {
        FURI_LOG_D("totp", "totp_load_keys::Line %d: %s", i, split_lines[i]);
    }

    // TODO: Ignore comment lines
    // TODO: Find the source of the crashing (crashes occasionally, seemingly random)
    // TODO: Some basic debugging shows that sometimes, an additional line is read after the last line
    // Check the `str_split` function to see if this is the cause? Maybe the read function?
    // If we can't find the issue, maybe we should expect a "END" or "EOL" or something like that
    // To indicate that we're at the end and ignore any content after that

    // We expect the first two lines to be the header and version lines
    // TODO: Validate the header and process as such

    app->key_count = 0;
    // Start at the third line (skip the header and version lines
    for(int i = 2; split_lines[i] != NULL; i++) {
        app->key_count++;
        FURI_LOG_D("totp", "totp_load_keys::Line %d: %s", i, split_lines[i]);
        char** split_fields = str_split(split_lines[i], ':');
        FURI_LOG_D("totp", "Setting key entry #%d's name to %s", i, split_fields[0]);
        app->key_entries[i-2].name = split_fields[0];
        FURI_LOG_D("totp", "Setting key entry #%d's secret to %s", i, split_fields[1]);
        app->key_entries[i-2].secret = split_fields[1];
        FURI_LOG_D("totp", "Setting key entry #%d's account to %s", i, split_fields[2]);
        app->key_entries[i-2].account = split_fields[2];
        FURI_LOG_D("totp", "Setting key entry #%d's duration to %s (converted to int %d)", i, split_fields[3], atoi(split_fields[3]));
        app->key_entries[i-2].duration = atoi(split_fields[3]);
        FURI_LOG_D("totp", "Setting key entry #%d's code_size to %s (converted to int %d)", i, split_fields[4], atoi(split_fields[4]));
        app->key_entries[i-2].code_size = atoi(split_fields[4]);
        free(split_fields);
    }


    free(key_file_path);
    free(split_lines);
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

    // Make our list of keys
    FURI_LOG_D("totp", "totp_app_alloc::Making our key list");
    totp_make_key_list(app);

    FURI_LOG_D("totp", "Starting to run...");
    int32_t status = totp_app_run(app);

    FURI_LOG_D("totp", "Freeing...");
    totp_app_free(app);
    FURI_LOG_D("totp", "Done!");

    FURI_LOG_D("totp", "Exiting...");
    return status;
}