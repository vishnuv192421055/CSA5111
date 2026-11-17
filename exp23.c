#include <stdio.h>
#include <stdint.h>

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

    c = ((c << 1) | (c >> 4)) & 0x1F;
    d = ((d << 1) | (d >> 4)) & 0x1F;
    *k1 = permute((c << 5) | d, p8, 8, 10);

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

void print_bin8(uint8_t b) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (b >> i) & 1);
        if (i == 4) printf(" ");
    }
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 23: S-DES in Counter (CTR) Mode                 \n");
    printf("============================================================\n\n");

    // Key: 01111 11101
    uint16_t key = 0b0111111101;
    uint8_t k1, k2;
    sdes_key_gen(key, &k1, &k2);

    // Plaintext blocks:
    // P1 = 0000 0001
    // P2 = 0000 0010
    // P3 = 0000 0100
    uint8_t p[3] = {0b00000001, 0b00000010, 0b00000100};
    uint8_t counter[3] = {0b00000000, 0b00000001, 0b00000010};

    printf("Test Vector Configuration:\n");
    printf("  Binary Key: 01111 11101\n");
    for (int i = 0; i < 3; i++) {
        printf("  Block %d: Plaintext = ", i + 1); print_bin8(p[i]);
        printf(", Counter = "); print_bin8(counter[i]);
        printf("\n");
    }
    printf("\n");

    // CTR Encryption: C_i = P_i ^ E(Counter_i)
    uint8_t c[3];
    uint8_t expected_c[3] = {0b00111000, 0b01001111, 0b00110010};

    printf("CTR Encryption Results:\n");
    for (int i = 0; i < 3; i++) {
        uint8_t pad = sdes_encrypt_byte(counter[i], k1, k2);
        c[i] = p[i] ^ pad;
        printf("  Block %d: Pad = ", i + 1); print_bin8(pad);
        printf(" -> Ciphertext = "); print_bin8(c[i]);
        printf(" (Expected: "); print_bin8(expected_c[i]);
        printf(") -> %s\n", (c[i] == expected_c[i]) ? "MATCH!" : "MISMATCH");
    }
    printf("\n");

    // CTR Decryption: P_i = C_i ^ E(Counter_i)
    printf("CTR Decryption Results:\n");
    for (int i = 0; i < 3; i++) {
        uint8_t pad = sdes_encrypt_byte(counter[i], k1, k2);
        uint8_t dec_p = c[i] ^ pad;
        printf("  Block %d: Decrypted = ", i + 1); print_bin8(dec_p);
        printf(" (Expected: "); print_bin8(p[i]);
        printf(") -> %s\n", (dec_p == p[i]) ? "MATCH!" : "MISMATCH");
    }
    printf("============================================================\n");
    return 0;
}
