/*
 * aes.h
 *
 *  Created on: 3 jun. 2025
 *      Author: miguelvladimirvargassanchez
 */

#ifndef TINY_AES_H
#define TINY_AES_H

#include <stddef.h>
#include <stdint.h>

#define AES_BLOCKLEN 16   // <-- agregamos esta línea

#define AES_KEYLEN   16  // AES-128


void aes_encrypt(const uint8_t *input, size_t input_len,
                 const uint8_t *key, const uint8_t *iv,
                 uint8_t *output, size_t *output_len);

void aes_decrypt(const uint8_t *input, size_t input_len,
                 const uint8_t *key, const uint8_t *iv,
                 uint8_t *output, size_t *output_len);
#endif // TINY_AES_H
