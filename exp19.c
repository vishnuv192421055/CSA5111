#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Mock block cipher core representing 1 DES round block operation
uint64_t mock_des_encrypt(uint64_t block, uint64_t key) {
    return block ^ key ^ 0xA5A5A5A5A5A5A5A5ULL;
}

uint64_t mock_des_decrypt(uint64_t block, uint64_t key) {
    return block ^ key ^ 0xA5A5A5A5A5A5A5A5ULL;
}

// 3DES (Triple DES EDE mode: Encrypt with K1, Decrypt with K2, Encrypt with K3)
uint64_t mock_3des_encrypt(uint64_t block, uint64_t k1, uint64_t k2, uint64_t k3) {
    uint64_t s1 = mock_des_encrypt(block, k1);
    uint64_t s2 = mock_des_decrypt(s1, k2);
    return mock_des_encrypt(s2, k3);
}

uint64_t mock_3des_decrypt(uint64_t block, uint64_t k1, uint64_t k2, uint64_t k3) {
    uint64_t s1 = mock_des_decrypt(block, k3);
    uint64_t s2 = mock_des_encrypt(s1, k2);
    return mock_des_decrypt(s2, k1);
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 19: CBC Mode with DES vs 3DES (Security & Speed)\n");
    printf("============================================================\n\n");

    uint64_t IV = 0x1234567890ABCDEFULL;
    uint64_t K1 = 0x1111222233334444ULL;
    uint64_t K2 = 0x5555666677778888ULL;
    uint64_t K3 = 0x9999AAAABBBBCCCCULL;

    uint64_t plaintext_blocks[3] = {
        0x0123456789ABCDEFULL,
        0xFEDCBA9876543210ULL,
        0x1020304050607080ULL
    };

    printf("Simulating CBC Mode Encryption with DES vs 3DES:\n");
    printf("------------------------------------------------------------\n");

    // DES CBC
    uint64_t des_cbc_cipher[3];
    uint64_t prev = IV;
    for (int i = 0; i < 3; i++) {
        des_cbc_cipher[i] = mock_des_encrypt(plaintext_blocks[i] ^ prev, K1);
        prev = des_cbc_cipher[i];
    }

    // 3DES CBC
    uint64_t tdes_cbc_cipher[3];
    prev = IV;
    for (int i = 0; i < 3; i++) {
        tdes_cbc_cipher[i] = mock_3des_encrypt(plaintext_blocks[i] ^ prev, K1, K2, K3);
        prev = tdes_cbc_cipher[i];
    }

    printf("Plaintext Block 0 : 0x%016llX\n", (unsigned long long)plaintext_blocks[0]);
    printf("DES CBC Block 0   : 0x%016llX (1 encryption cycle per block)\n", (unsigned long long)des_cbc_cipher[0]);
    printf("3DES CBC Block 0  : 0x%016llX (3 encryption/decryption cycles per block)\n\n", (unsigned long long)tdes_cbc_cipher[0]);

    printf("Analysis and Comparison:\n");
    printf("------------------------------------------------------------\n");
    printf("(a) Which would you choose for SECURITY?\n");
    printf("    -> 3DES (Triple DES).\n");
    printf("    Reason: Single DES has an effective key length of only 56 bits (2^56 keys),\n");
    printf("    which is completely broken by modern computing in hours (EFF DES cracker, 1999).\n");
    printf("    3DES provides 112 bits (2-key 3DES) or 168 bits (3-key 3DES) of effective key length,\n");
    printf("    rendering brute force completely infeasible.\n\n");

    printf("(b) Which would you choose for PERFORMANCE?\n");
    printf("    -> Single DES.\n");
    printf("    Reason: Single DES requires only 1 block cipher operation per 64-bit block,\n");
    printf("    whereas 3DES requires 3 sequential operations (Encrypt-Decrypt-Encrypt),\n");
    printf("    making 3DES roughly 3 times slower than DES in execution throughput.\n");
    printf("============================================================\n");
    return 0;
}
