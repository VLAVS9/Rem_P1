// aes.c

#include "aes.h"
#include <string.h>
#include <stdlib.h>

/*
 * Tiny-AES-c internals para AES-128 CBC.
 * Copiar íntegro este bloque tal cual del repositorio oficial:
 * https://github.com/kokke/tiny-AES-c
 */

static const uint8_t sbox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
  0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
  0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
  0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
  0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
  0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
  0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
  0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
  0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
  0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
  0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
  0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
  0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
  0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
  0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
  0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
  0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

static const uint8_t Rcon[11] = {
  0x00, 0x01, 0x02, 0x04,
  0x08, 0x10, 0x20, 0x40,
  0x80, 0x1B, 0x36
};


#define AES_KEYEXP_SIZE 176

struct AES_ctx {
    uint8_t RoundKey[AES_KEYEXP_SIZE];
    uint8_t Iv[AES_BLOCKLEN];
};

static uint8_t getSBoxValue(uint8_t num) {
    return sbox[num];
}

static void KeyExpansion(uint8_t *RoundKey, const uint8_t *Key) {
    unsigned i, j;
    uint8_t tempa[4];
    // Copiar clave original
    for (i = 0; i < AES_KEYLEN; ++i) {
        RoundKey[i] = Key[i];
    }
    // Expandir
    for (i = AES_KEYLEN; i < AES_KEYEXP_SIZE; i += 4) {
        for (j = 0; j < 4; ++j) {
            tempa[j] = RoundKey[(i - 4) + j];
        }
        if (i % AES_KEYLEN == 0) {
            // RotWord
            uint8_t t = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = t;
            // SubWord
            tempa[0] = getSBoxValue(tempa[0]);
            tempa[1] = getSBoxValue(tempa[1]);
            tempa[2] = getSBoxValue(tempa[2]);
            tempa[3] = getSBoxValue(tempa[3]);
            // Rcon
            tempa[0] = tempa[0] ^ Rcon[i / AES_KEYLEN];
        }
        for (j = 0; j < 4; ++j) {
            RoundKey[i + j] = RoundKey[i + j - AES_KEYLEN] ^ tempa[j];
        }
    }
}

static uint8_t xtime(uint8_t x) {
    return (uint8_t)((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00));
}
// Multiplicación en GF(2^8)
static uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint8_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a = xtime(a);
        b >>= 1;
    }
    return r;
}


static void SubBytes(uint8_t *state) {
    for (int i = 0; i < 16; ++i) {
        state[i] = getSBoxValue(state[i]);
    }
}

static void ShiftRows(uint8_t *state) {
    uint8_t temp;
    // Fila 1: rotar 1
    temp      = state[1];
    state[1]  = state[5];
    state[5]  = state[9];
    state[9]  = state[13];
    state[13] = temp;
    // Fila 2: rotar 2
    temp      = state[2];
    state[2]  = state[10];
    state[10] = temp;
    temp      = state[6];
    state[6]  = state[14];
    state[14] = temp;
    // Fila 3: rotar 3
    temp      = state[3];
    state[3]  = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7]  = temp;
}

static void MixColumns(uint8_t *state) {
    uint8_t Tmp, Tm, t;
    for (int i = 0; i < 4; ++i) {
        t   = state[i * 4 + 0];
        Tmp = state[i * 4 + 0] ^ state[i * 4 + 1] ^ state[i * 4 + 2] ^ state[i * 4 + 3];
        Tm  = state[i * 4 + 0] ^ state[i * 4 + 1];
        Tm  = (uint8_t)(xtime(Tm));
        state[i * 4 + 0] ^= Tm ^ Tmp;
        Tm  = state[i * 4 + 1] ^ state[i * 4 + 2];
        Tm  = (uint8_t)(xtime(Tm));
        state[i * 4 + 1] ^= Tm ^ Tmp;
        Tm  = state[i * 4 + 2] ^ state[i * 4 + 3];
        Tm  = (uint8_t)(xtime(Tm));
        state[i * 4 + 2] ^= Tm ^ Tmp;
        Tm  = state[i * 4 + 3] ^ t;
        Tm  = (uint8_t)(xtime(Tm));
        state[i * 4 + 3] ^= Tm ^ Tmp;
    }
}

static void AddRoundKey(uint8_t *state, const uint8_t *RoundKey) {
    for (int i = 0; i < 16; ++i) {
        state[i] ^= RoundKey[i];
    }
}

static void Cipher(uint8_t *state, const uint8_t *RoundKey) {
    AddRoundKey(state, RoundKey);
    for (uint8_t round = 1; round < 10; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, RoundKey + (round * 16));
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, RoundKey + 160);
}

static void XorWithIv(uint8_t *buf, const uint8_t *Iv) {
    for (int i = 0; i < AES_BLOCKLEN; ++i) {
        buf[i] ^= Iv[i];
    }
}

void AES_init_ctx_iv(struct AES_ctx *ctx, const uint8_t *key, const uint8_t *iv) {
    KeyExpansion(ctx->RoundKey, key);
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void AES_CBC_encrypt_buffer(struct AES_ctx *ctx, uint8_t *buf, size_t length) {
    uint8_t store[AES_BLOCKLEN];
    for (size_t i = 0; i < length; i += AES_BLOCKLEN) {
        memcpy(store, buf + i, AES_BLOCKLEN);
        XorWithIv(buf + i, ctx->Iv);
        Cipher(buf + i, ctx->RoundKey);
        memcpy(ctx->Iv, buf + i, AES_BLOCKLEN);
    }
}



// --------------------------------------------------
// Implementación AES-CBC decrypt + PKCS#7 unpad
// --------------------------------------------------

static uint8_t getSBoxInvert(uint8_t num) {
    static const uint8_t inv_sbox[256] = {
      0x52,0x09,0x6A,0xD5,0x30,0x36,0xA5,0x38,
      0xBF,0x40,0xA3,0x9E,0x81,0xF3,0xD7,0xFB,
      0x7C,0xE3,0x39,0x82,0x9B,0x2F,0xFF,0x87,
      0x34,0x8E,0x43,0x44,0xC4,0xDE,0xE9,0xCB,
      0x54,0x7B,0x94,0x32,0xA6,0xC2,0x23,0x3D,
      0xEE,0x4C,0x95,0x0B,0x42,0xFA,0xC3,0x4E,
      0x08,0x2E,0xA1,0x66,0x28,0xD9,0x24,0xB2,
      0x76,0x5B,0xA2,0x49,0x6D,0x8B,0xD1,0x25,
      0x72,0xF8,0xF6,0x64,0x86,0x68,0x98,0x16,
      0xD4,0xA4,0x5C,0xCC,0x5D,0x65,0xB6,0x92,
      0x6C,0x70,0x48,0x50,0xFD,0xED,0xB9,0xDA,
      0x5E,0x15,0x46,0x57,0xA7,0x8D,0x9D,0x84,
      0x90,0xD8,0xAB,0x00,0x8C,0xBC,0xD3,0x0A,
      0xF7,0xE4,0x58,0x05,0xB8,0xB3,0x45,0x06,
      0xD0,0x2C,0x1E,0x8F,0xCA,0x3F,0x0F,0x02,
      0xC1,0xAF,0xBD,0x03,0x01,0x13,0x8A,0x6B,
      0x3A,0x91,0x11,0x41,0x4F,0x67,0xDC,0xEA,
      0x97,0xF2,0xCF,0xCE,0xF0,0xB4,0xE6,0x73,
      0x96,0xAC,0x74,0x22,0xE7,0xAD,0x35,0x85,
      0xE2,0xF9,0x37,0xE8,0x1C,0x75,0xDF,0x6E,
      0x47,0xF1,0x1A,0x71,0x1D,0x29,0xC5,0x89,
      0x6F,0xB7,0x62,0x0E,0xAA,0x18,0xBE,0x1B,
      0xFC,0x56,0x3E,0x4B,0xC6,0xD2,0x79,0x20,
      0x9A,0xDB,0xC0,0xFE,0x78,0xCD,0x5A,0xF4,
      0x1F,0xDD,0xA8,0x33,0x88,0x07,0xC7,0x31,
      0xB1,0x12,0x10,0x59,0x27,0x80,0xEC,0x5F,
      0x60,0x51,0x7F,0xA9,0x19,0xB5,0x4A,0x0D,
      0x2D,0xE5,0x7A,0x9F,0x93,0xC9,0x9C,0xEF,
      0xA0,0xE0,0x3B,0x4D,0xAE,0x2A,0xF5,0xB0,
      0xC8,0xEB,0xBB,0x3C,0x83,0x53,0x99,0x61,
      0x17,0x2B,0x04,0x7E,0xBA,0x77,0xD6,0x26,
      0xE1,0x69,0x14,0x63,0x55,0x21,0x0C,0x7D
    };
    return inv_sbox[num];
}

static void InvSubBytes(uint8_t *state) {
    for (int i = 0; i < 16; ++i) {
        state[i] = getSBoxInvert(state[i]);
    }
}
// cambio de posiciones en cifrado inverso
static void InvShiftRows(uint8_t *state) {
    uint8_t temp;
    // Fila 1: rotar 3 hacia la derecha
    temp       = state[13];
    state[13]  = state[9];
    state[9]   = state[5];
    state[5]   = state[1];
    state[1]   = temp;
    // Fila 2: rotar 2
    temp       = state[2];
    state[2]   = state[10];
    state[10]  = temp;
    temp       = state[6];
    state[6]   = state[14];
    state[14]  = temp;
    // Fila 3: rotar 1
    temp       = state[3];
    state[3]   = state[7];
    state[7]   = state[11];
    state[11]  = state[15];
    state[15]  = temp;
}
// mezclado inverso
static void InvMixColumns(uint8_t *state) {
    for (int i = 0; i < 4; ++i) {
        uint8_t *col = state + 4*i;
        uint8_t a = col[0], b = col[1], c = col[2], d = col[3];
        col[0] = gf_mul(a,0x0e) ^ gf_mul(b,0x0b) ^ gf_mul(c,0x0d) ^ gf_mul(d,0x09);
        col[1] = gf_mul(a,0x09) ^ gf_mul(b,0x0e) ^ gf_mul(c,0x0b) ^ gf_mul(d,0x0d);
        col[2] = gf_mul(a,0x0d) ^ gf_mul(b,0x09) ^ gf_mul(c,0x0e) ^ gf_mul(d,0x0b);
        col[3] = gf_mul(a,0x0b) ^ gf_mul(b,0x0d) ^ gf_mul(c,0x09) ^ gf_mul(d,0x0e);
    }
}


static void AddRoundKeyInv(uint8_t *state, const uint8_t *RoundKey) {
    for (int i = 0; i < 16; ++i) {
        state[i] ^= RoundKey[i];
    }
}

static void InvCipher(uint8_t *state, const uint8_t *RoundKey) {
    AddRoundKeyInv(state, RoundKey + 160);
    for (uint8_t round = 9; round > 0; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKeyInv(state, RoundKey + (round * 16));
        InvMixColumns(state);
    }
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKeyInv(state, RoundKey);
}

/**
 * pkcs7_unpad:
 *   Verifica y quita el padding PKCS#7 al final de 'data' (longitud data_len).
 *   Devuelve -1 si el padding es inválido, o la nueva longitud (sin padding) si OK.
 */
static int pkcs7_unpad(uint8_t *data, size_t data_len) {
    if (data_len == 0 || (data_len % AES_BLOCKLEN) != 0) {
        return -1;
    }
    uint8_t pad_val = data[data_len - 1];
    if (pad_val == 0 || pad_val > AES_BLOCKLEN) {
        return -1;
    }
    for (size_t i = data_len - pad_val; i < data_len; ++i) {
        if (data[i] != pad_val) {
            return -1;
        }
    }
    return (int)(data_len - pad_val);
}

/**
 * aes_encrypt:
 *   1) Aplica PKCS#7 padding a 'input'.
 *   2) Inicializa AES-CBC con clave e IV.
 *   3) Cifra in-place usando AES_CBC_encrypt_buffer().
 *   4) Ajusta *output_len a longitud final (múltiplo de 16).
 */
static size_t pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t *padded) {
    size_t pad_len = AES_BLOCKLEN - (input_len % AES_BLOCKLEN);
    if (pad_len == 0) pad_len = AES_BLOCKLEN;
    size_t total_len = input_len + pad_len;

    memcpy(padded, input, input_len);
    for (size_t i = 0; i < pad_len; ++i) {
        padded[input_len + i] = (uint8_t)pad_len;
    }
    return total_len;
}

void aes_encrypt(const uint8_t *input, size_t input_len, const uint8_t *key, const uint8_t *iv, uint8_t *output, size_t *output_len) {
    size_t padded_len = pkcs7_pad(input, input_len, output);
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, output, padded_len);
    *output_len = padded_len;
}


/**
 * AES CBC decrypt buffer usa InvCipher y XorWithIv
 */
void AES_CBC_decrypt_buffer(struct AES_ctx *ctx, uint8_t *buf, size_t length)
{
    uint8_t prev_block[AES_BLOCKLEN];
    uint8_t curr_block[AES_BLOCKLEN];

    // Guardar IV inicial
    memcpy(prev_block, ctx->Iv, AES_BLOCKLEN);

    // Para cada bloque de 16 bytes
    for (size_t i = 0; i < length; i += AES_BLOCKLEN) {
        // Copiar ciphertext actual antes de desencriptar
        memcpy(curr_block, buf + i, AES_BLOCKLEN);
        InvCipher(buf + i, ctx->RoundKey);
        XorWithIv(buf + i, prev_block);


        memcpy(prev_block, curr_block, AES_BLOCKLEN);
    }

    // Actualizar IV en contexto (opcional)
    memcpy(ctx->Iv, prev_block, AES_BLOCKLEN);
}
/**
 * aes_decrypt:
 *   1) Descifra en AES-CBC (múltiplo de 16).
 *   2) Quita PKCS#7 padding (si es válido).
 *   Devuelve 1 si es OK, 0 en caso de error inválido.
 */
void aes_decrypt(const uint8_t *input, size_t input_len,
                 const uint8_t *key, const uint8_t *iv,
                 uint8_t *output, size_t *output_len)
{
    struct AES_ctx ctx;
    memcpy(output, input, input_len);
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, output, input_len);

    int unpadded = pkcs7_unpad(output, input_len);
    if (unpadded < 0) {

        *output_len = input_len;
    } else {
        *output_len = (size_t)unpadded;
        output[*output_len] = 0x00;
    }
}
