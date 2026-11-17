#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 8 // 8 bytes = 64 bits

// 1-bit padding (0x80 followed by 0x00s)
int padMessage(const uint8_t *in, int in_len, uint8_t *out) {
    memcpy(out, in, in_len);
    int pad_needed = BLOCK_SIZE - (in_len % BLOCK_SIZE);
    // If in_len is an exact multiple, pad_needed == BLOCK_SIZE (a full new block is added!)
    out[in_len] = 0x80; // 1 bit followed by 7 zeros
    for (int i = 1; i < pad_needed; i++) {
        out[in_len + i] = 0x00;
    }
    return in_len + pad_needed;
}

int unpadMessage(const uint8_t *in, int padded_len, uint8_t *out) {
    int unpadded_len = padded_len;
    // Scan backward for 0x80
    while (unpadded_len > 0 && in[unpadded_len - 1] == 0x00) {
        unpadded_len--;
    }
    if (unpadded_len > 0 && in[unpadded_len - 1] == 0x80) {
        unpadded_len--; // remove the 0x80 (1 bit)
    }
    memcpy(out, in, unpadded_len);
    return unpadded_len;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 21: Block Cipher Padding & Full Block Motivation\n");
    printf("============================================================\n\n");

    printf("Analysis of Padding in ECB, CBC, and CFB Modes:\n");
    printf("1. Block ciphers operate on fixed-size blocks (e.g. 64 or 128 bits).\n");
    printf("2. If plaintext length is not a multiple of the block size, padding is mandatory.\n");
    printf("3. Bit-padding rule: Append a single '1' bit followed by '0' bits until full.\n");
    printf("   (In byte-oriented data: append byte 0x80 followed by 0x00 bytes).\n\n");

    // Case 1: Incomplete block
    uint8_t msg1[5] = {'H', 'E', 'L', 'L', 'O'}; // 5 bytes (incomplete 8-byte block)
    uint8_t padded1[16], recovered1[16];
    int len1 = padMessage(msg1, 5, padded1);

    printf("Case 1: Message length NOT a multiple of block size (5 bytes):\n");
    printf("  Original : \"HELLO\" (5 bytes)\n");
    printf("  Padded   : ");
    for (int i = 0; i < len1; i++) printf("%02X ", padded1[i]);
    printf(" (Length = %d bytes, 1 block)\n", len1);
    int rlen1 = unpadMessage(padded1, len1, recovered1);
    recovered1[rlen1] = '\0';
    printf("  Recovered: \"%s\" (Length = %d bytes)\n\n", recovered1, rlen1);

    // Case 2: Complete block (exact multiple of 8 bytes)
    uint8_t msg2[8] = {'C', 'O', 'M', 'P', 'L', 'E', 'T', 'E'}; // 8 bytes
    uint8_t padded2[24], recovered2[24];
    int len2 = padMessage(msg2, 8, padded2);

    printf("Case 2: Message length ALREADY a multiple of block size (8 bytes):\n");
    printf("  Original : \"COMPLETE\" (8 bytes)\n");
    printf("  Padded   : ");
    for (int i = 0; i < len2; i++) printf("%02X ", padded2[i]);
    printf(" (Length = %d bytes, 2 blocks -> Extra padding block appended!)\n", len2);
    int rlen2 = unpadMessage(padded2, len2, recovered2);
    recovered2[rlen2] = '\0';
    printf("  Recovered: \"%s\" (Length = %d bytes)\n\n", recovered2, rlen2);

    printf("============================================================\n");
    printf("QUESTION: What is the motivation for including an entire padding\n");
    printf("          block when the message is already complete?\n");
    printf("------------------------------------------------------------\n");
    printf("ANSWER:\n");
    printf("  1. AMBIGUITY AVOIDANCE:\n");
    printf("     If no padding block were added when the plaintext is an exact multiple\n");
    printf("     of block size, the receiver cannot distinguish whether the final bytes\n");
    printf("     (e.g. 0x80 00 00) were genuine plaintext data or padding!\n");
    printf("  2. UNCONDITIONAL STRIPPING:\n");
    printf("     By always appending padding, the receiver knows with 100%% certainty\n");
    printf("     that the last block ALWAYS contains padding. The receiver unconditionally\n");
    printf("     locates the delimiter (0x80) and removes the padding without corrupting\n");
    printf("     messages that naturally end with null or control bytes.\n");
    printf("============================================================\n");
    return 0;
}
