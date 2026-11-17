#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// 64-bit CMAC Subkey Generation
void generate_subkeys_64(uint64_t L, uint64_t *K1, uint64_t *K2) {
    uint64_t Rb = 0x1BULL; // Constant for 64-bit block size

    // K1 = (L << 1) ^ (MSB(L) ? Rb : 0)
    if ((L >> 63) & 1) {
        *K1 = (L << 1) ^ Rb;
    } else {
        *K1 = (L << 1);
    }

    // K2 = (K1 << 1) ^ (MSB(K1) ? Rb : 0)
    if ((*K1 >> 63) & 1) {
        *K2 = (*K1 << 1) ^ Rb;
    } else {
        *K2 = (*K1 << 1);
    }
}

// 128-bit CMAC Subkey Generation (represented as high64 and low64)
typedef struct {
    uint64_t hi;
    uint64_t lo;
} uint128_t;

void shift_left_128(uint128_t in, uint128_t *out, bool *msb) {
    *msb = (in.hi >> 63) & 1;
    out->hi = (in.hi << 1) | (in.lo >> 63);
    out->lo = (in.lo << 1);
}

void generate_subkeys_128(uint128_t L, uint128_t *K1, uint128_t *K2) {
    uint64_t Rb = 0x87ULL; // Constant for 128-bit block size
    bool msb;

    shift_left_128(L, K1, &msb);
    if (msb) K1->lo ^= Rb;

    shift_left_128(*K1, K2, &msb);
    if (msb) K2->lo ^= Rb;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 31: CMAC Subkey Generation (NIST SP 800-38B)     \n");
    printf("============================================================\n\n");

    printf("(a) What constants are needed for block sizes of 64 and 128 bits?\n");
    printf("------------------------------------------------------------\n");
    printf("  1. For 64-bit block size  (e.g., 3DES) : R_64  = 0x000000000000001B (0x1B)\n");
    printf("     Irreducible polynomial: P(x) = x^64 + x^4 + x^3 + x + 1\n");
    printf("  2. For 128-bit block size (e.g., AES)  : R_128 = 0x00000000...00000087 (0x87)\n");
    printf("     Irreducible polynomial: P(x) = x^128 + x^7 + x^2 + x + 1\n\n");

    printf("(b) How the left shift and XOR accomplishes the desired result:\n");
    printf("------------------------------------------------------------\n");
    printf("  - In Galois Field GF(2^b), multiplication of a polynomial A(x) by 'x'\n");
    printf("    corresponds directly to shifting its binary representation left by 1 bit.\n");
    printf("  - If the MSB (coefficient of x^(b-1)) was 0, no overflow occurs.\n");
    printf("  - If the MSB was 1, multiplying by x yields x^b, which exceeds the field degree.\n");
    printf("  - Reduction modulo P(x) is performed: subtracting x^b in GF(2) is identical\n");
    printf("    to bitwise XORing with the constant R_b.\n\n");

    printf("Demonstration with 64-Bit Block:\n");
    printf("------------------------------------------------------------\n");
    // L = E_K(0^64)
    uint64_t L_64 = 0x8A5B3C4D5E6F7081ULL; // MSB is 1 (0x8...)
    uint64_t K1_64, K2_64;
    generate_subkeys_64(L_64, &K1_64, &K2_64);

    printf("  L  = E_K(0^64) : 0x%016llX (MSB = %d)\n", (unsigned long long)L_64, (int)(L_64 >> 63));
    printf("  K1 = 2 * L     : 0x%016llX (MSB = %d)\n", (unsigned long long)K1_64, (int)(K1_64 >> 63));
    printf("  K2 = 2 * K1    : 0x%016llX\n\n", (unsigned long long)K2_64);

    printf("Demonstration with 128-Bit Block (AES Standard):\n");
    printf("------------------------------------------------------------\n");
    uint128_t L_128 = {0x7DF76B0C1FE57445ULL, 0xAD345D00A1456ABCULL}; // MSB is 0
    uint128_t K1_128, K2_128;
    generate_subkeys_128(L_128, &K1_128, &K2_128);

    printf("  L  = E_K(0^128): 0x%016llX %016llX (MSB = %d)\n",
           (unsigned long long)L_128.hi, (unsigned long long)L_128.lo, (int)(L_128.hi >> 63));
    printf("  K1 = 2 * L     : 0x%016llX %016llX (MSB = %d)\n",
           (unsigned long long)K1_128.hi, (unsigned long long)K1_128.lo, (int)(K1_128.hi >> 63));
    printf("  K2 = 2 * K1    : 0x%016llX %016llX\n",
           (unsigned long long)K2_128.hi, (unsigned long long)K2_128.lo);
    printf("============================================================\n");
    return 0;
}
