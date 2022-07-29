#include "totp_view.h"

//#include <toolbox/path.h>


#include "totp_helpers.h"
#include "totp_backend.h"
//#include "totp_backend/totp_backend.h"

// Callbacks
static void view_totp_draw_callback(Canvas* canvas, void* _model) {
    ViewTotpModel* model = _model;

    char* text0 = malloc(sizeof(char) * 50);
    char* text1 = malloc(sizeof(char) * 50);
    char* text2 = malloc(sizeof(char) * 50);
    char* text3 = malloc(sizeof(char) * 50);
    char* text4 = malloc(sizeof(char) * 50);
    //    TotpCalculationResult result = totp_generate_totp_NEW(model->entry.secret, 30, 6);
    TotpCalculationResult result = totp_generate_totp(model->entry.secret);

    if(result.code == NULL) {
        sprintf(text0, "!!! ERROR!!! ");
        sprintf(text1, "INVALID KEY!");
        sprintf(text2, "!!! ERROR!!! ");
        canvas_draw_str(canvas, 0, 12, text0);
        canvas_draw_str(canvas, 0, 24, text1);
        canvas_draw_str(canvas, 0, 36, text2);
    } else {
        sprintf(text0, "Name: %s", model->entry.name);
        sprintf(text1, "Account: %s", model->entry.account);
        sprintf(text3, "Code: %s", result.code);
        sprintf(text4, "Expires in: %d seconds", result.expires_in+1);

        canvas_draw_str(canvas, 0, 12, text0);
        canvas_draw_str(canvas, 0, 24, text1);
        canvas_draw_str(canvas, 0, 36, text3);
        canvas_draw_str(canvas, 0, 48, text4);

        // Do this without getting the date/time twice

        canvas_draw_box(canvas, 0, 52, (result.expires_in * 4.414), 10);
    }
    free(text0);
    free(text1);
    free(text2);
    free(text3);
    free(text4);
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