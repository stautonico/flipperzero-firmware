#pragma once

#include "nfc_device.h"

typedef struct NfcWorker NfcWorker;

typedef enum {
    // Init states
    NfcWorkerStateNone,
    NfcWorkerStateBroken,
    NfcWorkerStateReady,
    // Main worker states
    NfcWorkerStateRead,
    NfcWorkerStateUidEmulate,
    NfcWorkerStateMfUltralightEmulate,
    NfcWorkerStateMfClassicEmulate,
    NfcWorkerStateReadMfUltralightReadAuth,
    NfcWorkerStateMfClassicDictAttack,
    NfcWorkerStateAnalyzeReader,
    // Debug
    NfcWorkerStateEmulateApdu,
    NfcWorkerStateField,
    // Transition
    NfcWorkerStateStop,
} NfcWorkerState;

typedef enum {
    // Reserve first 50 events for application events
    NfcWorkerEventReserved = 50,

    // Nfc read events
    NfcWorkerEventReadUidNfcB,
    NfcWorkerEventReadUidNfcV,
    NfcWorkerEventReadUidNfcF,
    NfcWorkerEventReadUidNfcA,
    NfcWorkerEventReadMfUltralight,
    NfcWorkerEventReadMfDesfire,
    NfcWorkerEventReadMfClassicDone,
    NfcWorkerEventReadMfClassicLoadKeyCache,
    NfcWorkerEventReadMfClassicDictAttackRequired,
    NfcWorkerEventReadBankCard,

    // Nfc worker common events
    NfcWorkerEventSuccess,
    NfcWorkerEventFail,
    NfcWorkerEventAborted,
    NfcWorkerEventCardDetected,
    NfcWorkerEventNoCardDetected,
    NfcWorkerEventWrongCardDetected,

    // Mifare Classic events
    NfcWorkerEventNoDictFound,
    NfcWorkerEventNewSector,
    NfcWorkerEventNewDictKeyBatch,
    NfcWorkerEventFoundKeyA,
    NfcWorkerEventFoundKeyB,

<<<<<<< HEAD
    // Mifare Ultralight/NTAG events
    NfcWorkerEventMfUltralightPassKey, // NFC worker requesting manual key
    NfcWorkerEventMfUltralightPwdAuth, // Reader sent auth command
||||||| [FL-2764] SubGhz: fix CAME, Chamberlain potocol (#1650)
    // Mifare Ultralight events
    NfcWorkerEventMfUltralightPassKey,
=======
    // Mifare Ultralight/NTAG events
    NfcWorkerEventMfUltralightPassKey, // NFC worker requesting manual key
    NfcWorkerEventMfUltralightPwdAuth, // Reader sent auth command

    // Detect Reader events
    NfcWorkerEventDetectReaderMfkeyCollected,

>>>>>>> unleashed
} NfcWorkerEvent;

typedef bool (*NfcWorkerCallback)(NfcWorkerEvent event, void* context);

NfcWorker* nfc_worker_alloc();

NfcWorkerState nfc_worker_get_state(NfcWorker* nfc_worker);

void* nfc_worker_get_event_data(NfcWorker* nfc_worker);

void nfc_worker_free(NfcWorker* nfc_worker);

void nfc_worker_start(
    NfcWorker* nfc_worker,
    NfcWorkerState state,
    NfcDeviceData* dev_data,
    NfcWorkerCallback callback,
    void* context);

void nfc_worker_stop(NfcWorker* nfc_worker);
