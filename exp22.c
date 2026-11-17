#include <stdio.h>
#include <stdint.h>

// S-DES tables
static const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

static const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

uint16_t permute(uint16_t in, const int *table, int n, int in_bits) {
    uint16_t out = 0;
    for (int i = 0; i < n; i++) {
        int bit = (in >> (in_bits - table[i])) & 1;
        out = (out << 1) | bit;
    }
    return out;
}

void sdes_key_gen(uint16_t key10, uint8_t *k1, uint8_t *k2) {
    int p10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
    int p8[]  = {6, 3, 7, 4, 8, 5, 10, 9};
    uint16_t p = permute(key10, p10, 10, 10);
    uint8_t c = (p >> 5) & 0x1F;
    uint8_t d = p & 0x1F;

    // LS-1
    c = ((c << 1) | (c >> 4)) & 0x1F;
    d = ((d << 1) | (d >> 4)) & 0x1F;
    *k1 = permute((c << 5) | d, p8, 8, 10);

    // LS-2
    c = ((c << 2) | (c >> 3)) & 0x1F;
    d = ((d << 2) | (d >> 3)) & 0x1F;
    *k2 = permute((c << 5) | d, p8, 8, 10);
}

uint8_t f_k(uint8_t r, uint8_t sk) {
    int ep[] = {4, 1, 2, 3, 2, 3, 4, 1};
    int p4[] = {2, 4, 3, 1};
    uint8_t exp = permute(r, ep, 8, 4) ^ sk;
    uint8_t left = (exp >> 4) & 0x0F;
    uint8_t right = exp & 0x0F;

    int r0 = ((left & 0x08) >> 2) | (left & 0x01);
    int c0 = (left >> 1) & 0x03;
    uint8_t s0_val = S0[r0][c0];

    int r1 = ((right & 0x08) >> 2) | (right & 0x01);
    int c1 = (right >> 1) & 0x03;
    uint8_t s1_val = S1[r1][c1];

    return permute((s0_val << 2) | s1_val, p4, 4, 4);
}

uint8_t sdes_encrypt_byte(uint8_t pt, uint8_t k1, uint8_t k2) {
    int ip[] = {2, 6, 3, 1, 4, 8, 5, 7};
    int ip_inv[] = {4, 1, 3, 5, 7, 2, 8, 6};

    uint8_t t = permute(pt, ip, 8, 8);
    uint8_t l = (t >> 4) & 0x0F;
    uint8_t r = t & 0x0F;

    l ^= f_k(r, k1);
    uint8_t temp = l; l = r; r = temp;
    l ^= f_k(r, k2);

    return permute((l << 4) | r, ip_inv, 8, 8);
}

uint8_t sdes_decrypt_byte(uint8_t ct, uint8_t k1, uint8_t k2) {
    return sdes_encrypt_byte(ct, k2, k1);
}

void print_bin8(uint8_t b) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (b >> i) & 1);
        if (i == 4) printf(" ");
    }
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 22: S-DES in Cipher Block Chaining (CBC) Mode   \n");
    printf("============================================================\n\n");

    // Given parameters:
    // Key: 01111 11101 (10 bits) = 0x1FD
    uint16_t key = 0b0111111101;
    // IV: 1010 1010 = 0xAA
    uint8_t iv = 0b10101010;
    // Plaintext: 0000 0001 (0x01) and 0010 0011 (0x23)
    uint8_t p[2] = {0b00000001, 0b00100011};

    uint8_t k1, k2;
    sdes_key_gen(key, &k1, &k2);

    printf("Test Vector Configuration:\n");
    printf("  Binary Key  : 01111 11101 (K1 = 0x%02X, K2 = 0x%02X)\n", k1, k2);
    printf("  Binary IV   : "); print_bin8(iv); printf("\n");
    printf("  Plaintext P1: "); print_bin8(p[0]); printf("\n");
    printf("  Plaintext P2: "); print_bin8(p[1]); printf("\n\n");

    // CBC Encryption:
    // C1 = E_k(P1 ^ IV)
    // C2 = E_k(P2 ^ C1)
    uint8_t c[2];
    c[0] = sdes_encrypt_byte(p[0] ^ iv, k1, k2);
    c[1] = sdes_encrypt_byte(p[1] ^ c[0], k1, k2);

    printf("CBC Encryption Results:\n");
    printf("  Block 1: E(P1 ^ IV) -> Ciphertext C1: "); print_bin8(c[0]);
    printf(" (Expected: 1111 0100) -> %s\n", (c[0] == 0b11110100) ? "MATCH!" : "MISMATCH");
    printf("  Block 2: E(P2 ^ C1) -> Ciphertext C2: "); print_bin8(c[1]);
    printf(" (Expected: 0000 1011) -> %s\n\n", (c[1] == 0b00001011) ? "MATCH!" : "MISMATCH");

    // CBC Decryption:
    // P1 = D_k(C1) ^ IV
    // P2 = D_k(C2) ^ C1
    uint8_t dec_p[2];
    dec_p[0] = sdes_decrypt_byte(c[0], k1, k2) ^ iv;
    dec_p[1] = sdes_decrypt_byte(c[1], k1, k2) ^ c[0];

    printf("CBC Decryption Results:\n");
    printf("  Decrypted P1: "); print_bin8(dec_p[0]);
    printf(" (Expected: 0000 0001) -> %s\n", (dec_p[0] == p[0]) ? "MATCH!" : "MISMATCH");
    printf("  Decrypted P2: "); print_bin8(dec_p[1]);
    printf(" (Expected: 0010 0011) -> %s\n", (dec_p[1] == p[1]) ? "MATCH!" : "MISMATCH");
    printf("============================================================\n");
    return 0;
}
