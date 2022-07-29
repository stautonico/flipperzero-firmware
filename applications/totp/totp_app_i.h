#pragma once

#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>

typedef struct TotpKeyEntry {
    const char* name;
    const char* secret;
    const char* account;
} TotpKeyEntry;

typedef struct ViewTotp {
    View* view;
} ViewTotp;

typedef struct TotpApp {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    ViewTotp* view_totp;
    VariableItemList* variable_item_list;
    Submenu* submenu;

    // State goes here?
} TotpApp;

typedef struct ViewTotpModel {
    TotpKeyEntry entry;
} ViewTotpModel; // State

typedef enum { TotpViewStart, TotpViewKey } TotpViews;
