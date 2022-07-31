#include "totp_view.h"

//#include <toolbox/path.h>

#include "totp_helpers.h"
#include "totp_backend.h"
//#include "totp_backend/totp_backend.h"

// Callbacks
static void view_totp_draw_callback(Canvas* canvas, void* _model) {
    ViewTotpModel* model = _model;

    // Clear out the canvas and set initial drawing state
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontBigNumbers);

    TotpCalculationResult result = totp_generate_totp(model->entry.secret);

    if(result.code == NULL) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "!!! ERROR !!!");
        canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignCenter, "INVALID SECRET KEY");

        // +3 for the two quotes and the null terminator
        char* error_message = malloc(sizeof(char) * (strlen(model->entry.name) + 2) + 1);
        sprintf(error_message, "\"%s\"", model->entry.name);

        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(
            canvas, 64, 36, AlignCenter, AlignCenter, "Check the secret key for");
        canvas_draw_str_aligned(canvas, 64, 48, AlignCenter, AlignCenter, error_message);

        free(error_message);
    } else {
        canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignCenter, result.code);

        char* expiry = malloc(sizeof(char) * 50);

        // Write "second" if expiry is 1, "seconds" if expiry is > 1
        if(result.expires_in == 1) {
            sprintf(expiry, "Expires in %d second", result.expires_in);
        } else {
            sprintf(expiry, "Expires in %d seconds", result.expires_in);
        }

        // Change back to the normal font to draw the expiration time and account name
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 4, AlignCenter, AlignCenter, model->entry.name);
        canvas_draw_str_aligned(canvas, 64, 16, AlignCenter, AlignCenter, model->entry.account);
        canvas_draw_str_aligned(canvas, 64, 44, AlignCenter, AlignCenter, expiry);

        canvas_draw_box(canvas, 0, 52, ((result.expires_in - 1) * 4.414), 10);

        free(expiry);
        // Make sure to free the result.code string (if not NULL)
        free(result.code);
    }
}

static bool view_totp_input_callback(InputEvent* event, void* context) {
    ViewTotp* instance = context;

    bool consumed = false;
    // Handle input

    return false;
}

// Allocators/Deallocators
ViewTotp* view_totp_alloc() {
    FURI_LOG_D("totp", "view_totp_alloc::Allocating view object");
    ViewTotp* instance = malloc(sizeof(ViewTotp));

    FURI_LOG_D("totp", "view_totp_alloc::Allocating view");
    instance->view = view_alloc();
    FURI_LOG_D("totp", "view_totp_alloc::Setting context");
    view_set_context(instance->view, instance);
    FURI_LOG_D("totp", "view_totp_alloc::Allocating model");
    view_allocate_model(instance->view, ViewModelTypeLockFree, sizeof(ViewTotpModel));
    FURI_LOG_D("totp", "view_totp_alloc::Setting draw and input callbacks");
    view_set_draw_callback(instance->view, view_totp_draw_callback);
    view_set_input_callback(instance->view, view_totp_input_callback);
    // Do we need input and enter callbacks?
    //    view_set_enter_callback(instance->view, view_totp_enter_callback);
    //    view_set_exit_callback(instance->view, view_totp_exit_callback);

    FURI_LOG_D("totp", "view_totp_alloc::Done allocating view!");
    return instance;
}

void view_totp_free(ViewTotp* instance) {
    furi_assert(instance);

    view_free(instance->view);
    free(instance);
}

View* view_totp_get_view(ViewTotp* instance) {
    furi_assert(instance);
    return instance->view;
}