#pragma once

#include <stdbool.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>


typedef struct TotpKeyEntry {
    const char* name;
    const unsigned char* secret;
    const char* account;
    int duration; // Time step of the TOTP algorithm (default 30)
    int code_size; // Size of the TOTP key (default 6)
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
    // TODO: Make this a dynamic array
    TotpKeyEntry key_entries[32];
    int key_count;

    // State goes here?
} TotpApp;

typedef struct ViewTotpModel {
    TotpKeyEntry entry;
} ViewTotpModel; // State

typedef enum { TotpViewStart, TotpViewKey, TotpViewAbout } TotpViews;
