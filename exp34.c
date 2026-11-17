#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8

// Mock block cipher
uint64_t encrypt_block(uint64_t b, uint64_t k) { return b ^ k ^ 0x0123456789ABCDEFULL; }
uint64_t decrypt_block(uint64_t b, uint64_t k) { return b ^ k ^ 0x0123456789ABCDEFULL; }

int main() {
    printf("============================================================\n");
    printf(" Experiment 34: ECB, CBC, and CFB Modes with Block Padding  \n");
    printf("============================================================\n\n");

    uint64_t key = 0xFEEDFACECAFEBEEFULL;
    uint64_t iv  = 0x1122334455667788ULL;

    // Plaintext of 12 bytes: Needs padding to become multiple of 8 bytes (2 blocks = 16 bytes)
    uint8_t raw_msg[12] = "Confidential";
    uint8_t padded_msg[16];
    memcpy(padded_msg, raw_msg, 12);
    // 1-bit padding: append 0x80 then 0x00
    padded_msg[12] = 0x80;
    padded_msg[13] = 0x00;
    padded_msg[14] = 0x00;
    padded_msg[15] = 0x00;

    uint64_t P[2];
    memcpy(&P[0], &padded_msg[0], 8);
    memcpy(&P[1], &padded_msg[8], 8);

    printf("Plaintext: \"Confidential\" (12 bytes)\n");
    printf("Padded   : 16 bytes (2 full 8-byte blocks)\n\n");

    // 1. ECB Mode
    uint64_t C_ecb[2];
    C_ecb[0] = encrypt_block(P[0], key);
    C_ecb[1] = encrypt_block(P[1], key);
    printf("1. ECB Mode:\n");
    printf("   C1 = Enc(P1) = 0x%016llX\n", (unsigned long long)C_ecb[0]);
    printf("   C2 = Enc(P2) = 0x%016llX\n\n", (unsigned long long)C_ecb[1]);

    // 2. CBC Mode
    uint64_t C_cbc[2];
    C_cbc[0] = encrypt_block(P[0] ^ iv, key);
    C_cbc[1] = encrypt_block(P[1] ^ C_cbc[0], key);
    printf("2. CBC Mode:\n");
    printf("   C1 = Enc(P1 ^ IV) = 0x%016llX\n", (unsigned long long)C_cbc[0]);
    printf("   C2 = Enc(P2 ^ C1) = 0x%016llX\n\n", (unsigned long long)C_cbc[1]);

    // 3. CFB Mode (Cipher Feedback)
    uint64_t C_cfb[2];
    C_cfb[0] = P[0] ^ encrypt_block(iv, key);
    C_cfb[1] = P[1] ^ encrypt_block(C_cfb[0], key);
    printf("3. CFB Mode:\n");
    printf("   C1 = P1 ^ Enc(IV) = 0x%016llX\n", (unsigned long long)C_cfb[0]);
    printf("   C2 = P2 ^ Enc(C1) = 0x%016llX\n\n", (unsigned long long)C_cfb[1]);

    printf("Motivation for Including a Padding Block when Padding is Not Needed:\n");
    printf("--------------------------------------------------------------------\n");
    printf("  If an unpadded message naturally happens to be an exact multiple of the\n");
    printf("  block size (e.g. 16 bytes), omitting padding creates an AMBIGUITY:\n");
    printf("  The receiver cannot tell whether the trailing bytes of the last block are\n");
    printf("  valid payload data or padding bytes.\n");
    printf("  By ALWAYS appending a dummy padding block when the payload is already complete,\n");
    printf("  the receiver can UNCONDITIONALLY strip the padding without inspecting context!\n");
    printf("============================================================\n");
    return 0;
}
