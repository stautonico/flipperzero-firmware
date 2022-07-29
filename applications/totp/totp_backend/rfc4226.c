#include "rfc4226.h"

#include <math.h>
#include <string.h>

// Bits and pieces of this is taken from astra's totp and hmac code,
// some is from fmount's totp code, and the rest is mine
// I'm really not smart enough to fully implement totp and its required algorithms,
// but I understand some of it, so I'm going to do as much as I can on my own
// and use the rest from smarter people.
// Thanks astra and fmount!

#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

#define SHA1_K0 0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

uint8_t hmac_init_state[] = {
    0x01, 0x23, 0x45, 0x67, // H0
    0x89, 0xab, 0xcd, 0xef, // H1
    0xfe, 0xdc, 0xba, 0x98, // H2
    0x76, 0x54, 0x32, 0x10, // H3
    0xf0, 0xe1, 0xd2, 0xc3 // H4
};

// I don't like the whole 'global state' thing, so I'm just going to create a struct
// that would contain all our 'global state' and pass it around.
// This may be a really bad idea
typedef struct TotpHmacState {
    union _buffer {
        uint8_t b[BLOCK_LENGTH];
        uint32_t w[BLOCK_LENGTH / 4];
    } buffer;
    union _state {
        uint8_t b[HASH_LENGTH];
        uint32_t w[HASH_LENGTH / 4];
    } state;
    uint8_t buffer_offset;
    uint32_t byte_count;
    uint8_t key_buffer[BLOCK_LENGTH];
    uint8_t inner_hash[HASH_LENGTH];
} TotpHmacState;

TotpHmacState totp_init_hmac_state() {
    TotpHmacState state;
    memcpy(state.state.b, hmac_init_state, HASH_LENGTH);
    state.buffer_offset = 0;
    state.byte_count = 0;
    return state;
}

uint32_t totp_hmac_rol32(uint32_t number, uint8_t bits) {
    return ((number << bits) | (uint32_t)(number >> (32 - bits)));
}

void totp_hmac_hash_block(TotpHmacState* state) {
    uint32_t a, b, c, d, e, t;

    a = state->state.w[0];
    b = state->state.w[1];
    c = state->state.w[2];
    d = state->state.w[3];
    e = state->state.w[4];

    for(uint8_t i = 0; i < 80; i++) {
        if(i >= 16) {
            t = state->buffer.w[(i + 13) & 15] ^ state->buffer.w[(i + 8) & 15] ^
                state->buffer.w[(i + 2) & 15] ^ state->buffer.w[i & 15];
            state->buffer.w[i & 15] = totp_hmac_rol32(t, 1);
        }

        if(i < 20) {
            t = (d ^ (b & (c ^ d))) + SHA1_K0;
        } else if(i < 40) {
            t = (b ^ c ^ d) + SHA1_K20;
        } else if(i < 60) {
            t = ((b & c) | (d & (b | c))) + SHA1_K40;
        } else {
            t = (b ^ c ^ d) + SHA1_K60;
        }

        t += totp_hmac_rol32(a, 5) + e + state->buffer.w[i & 15];
        e = d;
        d = c;
        c = totp_hmac_rol32(b, 30);
        b = a;
        a = t;
    }

    state->state.w[0] += a;
    state->state.w[1] += b;
    state->state.w[2] += c;
    state->state.w[3] += d;
    state->state.w[4] += e;
}

void totp_hmac_add_uncounted(uint8_t data, TotpHmacState* state) {
    state->buffer.b[state->buffer_offset ^ 3] = data;
    state->buffer_offset++;
    if(state->buffer_offset == BLOCK_LENGTH) {
        totp_hmac_hash_block(state);
        state->buffer_offset = 0;
    }
}

void totp_hmac_write(uint8_t data, TotpHmacState* state) {
    ++(state->byte_count);
    totp_hmac_add_uncounted(data, state);
}

void totp_hmac_pad(TotpHmacState* state) {
    // Pad with 0x80 followed by 0x00 until the end of the block
    totp_hmac_add_uncounted(0x80, state);
    while(state->buffer_offset != 56) totp_hmac_add_uncounted(0x00, state);

    // Append length in the last 8 bytes
    totp_hmac_add_uncounted(0, state); // We're only using 32 bit lengths
    totp_hmac_add_uncounted(0, state); // But SHA-1 supports 64 bit lengths
    totp_hmac_add_uncounted(0, state); // So zero pad the top bits
    totp_hmac_add_uncounted(state->byte_count >> 29, state); // Shifting to multiply by 8
    totp_hmac_add_uncounted(
        state->byte_count >> 21, state); // as SHA-1 supports bitstreams as well as
    totp_hmac_add_uncounted(state->byte_count >> 13, state); // byte.
    totp_hmac_add_uncounted(state->byte_count >> 5, state);
    totp_hmac_add_uncounted(state->byte_count << 3, state);
}

uint8_t* totp_hmac_result(TotpHmacState* state) {
    totp_hmac_pad(state);

    for(uint8_t i = 0; i < 5; i++) {
        uint32_t a, b;
        a = state->state.w[i];
        b = a << 24;
        b |= (a << 8) & 0x00ff0000;
        b |= (a >> 8) & 0x0000ff00;
        b |= a >> 24;
        state->state.w[i] = b;
    }

    return state->state.b;
}

void totp_hmac_write_array(uint8_t* buffer, uint8_t size, TotpHmacState* state) {
    while(size--) {
        totp_hmac_write(*buffer++, state);
    }
}

uint8_t* totp_hmac(uint8_t* key, int keylen, uint64_t interval) {
    // Since we don't have openssl, we have to use what Astra did.
    // This basically gets the SHA1-hmac of the key

    TotpHmacState state = totp_init_hmac_state();

    if(keylen > BLOCK_LENGTH) {
        for(; keylen--;) totp_hmac_write(*key++, &state);
        memcpy(state.key_buffer, totp_hmac_result(&state), HASH_LENGTH);
    } else {
        memcpy(state.key_buffer, key, keylen);
    }

    for(uint8_t i = 0; i < BLOCK_LENGTH; i++) {
        totp_hmac_write(state.key_buffer[i] ^ HMAC_IPAD, &state);
    }

    memset(state.key_buffer, 0, BLOCK_LENGTH);

    uint8_t i;
    memcpy(state.inner_hash, totp_hmac_result(&state), HASH_LENGTH);

    // Reset the state
    state = totp_init_hmac_state();

    for(i = 0; i < BLOCK_LENGTH; i++) totp_hmac_write(state.key_buffer[i] ^ HMAC_OPAD, &state);
    for(i = 0; i < HASH_LENGTH; i++) totp_hmac_write(state.inner_hash[i], &state);

    return totp_hmac_result(&state);
}

uint32_t totp_mod_hotp(uint32_t bin_code, int digits) {
    int power = pow(10, digits);
    uint32_t otp = bin_code % power;

    return otp;
}

uint32_t totp_internal_HOPT(uint8_t* key, uint32_t keylen, uint64_t interval, int digits) {
    // Taken from c_opt/rfc4226.c
    uint8_t* digest;
    uint32_t result;
    uint32_t endianness;

    endianness = 0xdeadbeef;
    if((*(const uint8_t*)&endianness) == 0xef) {
        interval = ((interval & 0x00000000ffffffff) << 32) |
                   ((interval & 0xffffffff00000000) >> 32);
        interval = ((interval & 0x0000ffff0000ffff) << 16) |
                   ((interval & 0xffff0000ffff0000) >> 16);
        interval = ((interval & 0x00ff00ff00ff00ff) << 8) | ((interval & 0xff00ff00ff00ff00) >> 8);
    };

    // TODO: Check this against the openssl implementation

    //First Phase, get the digest of the message using the provided key ...
    digest = (uint8_t*)totp_hmac(key, keylen, interval);

    //Second Phase, get the dbc from the algorithm
    uint32_t dbc = totp_internal_DT(digest);

    //Third Phase: calculate the mod_k of the dbc to get the correct number
    result = totp_mod_hotp(dbc, digits);

    return result;
}

uint32_t totp_internal_DT(uint8_t* digest) {
    // Taken from c_opt/rfc4226.c
    uint64_t offset;
    uint32_t bin_code;

    // dynamically truncates hash
    offset = digest[19] & 0x0f;

    bin_code = (digest[offset] & 0x7f) << 24 | (digest[offset + 1] & 0xff) << 16 |
               (digest[offset + 2] & 0xff) << 8 | (digest[offset + 3] & 0xff);

    return bin_code;
}