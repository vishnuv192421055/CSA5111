#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Simple reversible block cipher for demonstration
uint8_t enc_block(uint8_t p, uint8_t k) { return (p + k) % 256; }
uint8_t dec_block(uint8_t c, uint8_t k) { return (c - k + 256) % 256; }

int main() {
    printf("============================================================\n");
    printf(" Experiment 20: Error Propagation in ECB and CBC Modes      \n");
    printf("============================================================\n\n");

    uint8_t key = 0x5A;
    uint8_t iv = 0x33;
    uint8_t P[4] = {0x10, 0x20, 0x30, 0x40}; // 4 blocks: P1, P2, P3, P4
    uint8_t C_ecb[4], C_cbc[4];

    // Encryption
    for (int i = 0; i < 4; i++) {
        C_ecb[i] = enc_block(P[i], key);
    }
    uint8_t prev = iv;
    for (int i = 0; i < 4; i++) {
        C_cbc[i] = enc_block(P[i] ^ prev, key);
        prev = C_cbc[i];
    }

    printf("Original Plaintext Blocks : P1=0x%02X, P2=0x%02X, P3=0x%02X, P4=0x%02X\n",
           P[0], P[1], P[2], P[3]);
    printf("Transmitted CBC Ciphertext: C1=0x%02X, C2=0x%02X, C3=0x%02X, C4=0x%02X\n\n",
           C_cbc[0], C_cbc[1], C_cbc[2], C_cbc[3]);

    printf("============================================================\n");
    printf("SCENARIO (a): Transmission Bit Error in Ciphertext C1\n");
    printf("Suppose bit 0 of C1 is flipped during transmission: C1' = C1 ^ 0x01\n");
    uint8_t C_cbc_err[4];
    for (int i = 0; i < 4; i++) C_cbc_err[i] = C_cbc[i];
    C_cbc_err[0] ^= 0x01; // flip 1 bit in C1

    // Decrypt at receiver: P_i = Dec(C_i) ^ C_{i-1}
    uint8_t P_recovered[4];
    P_recovered[0] = dec_block(C_cbc_err[0], key) ^ iv;
    P_recovered[1] = dec_block(C_cbc_err[1], key) ^ C_cbc_err[0];
    P_recovered[2] = dec_block(C_cbc_err[2], key) ^ C_cbc_err[1];
    P_recovered[3] = dec_block(C_cbc_err[3], key) ^ C_cbc_err[2];

    printf("Receiver Decrypted Plaintext Blocks:\n");
    for (int i = 0; i < 4; i++) {
        printf("  P%d: Received=0x%02X, Original=0x%02X -> %s\n",
               i + 1, P_recovered[i], P[i],
               (P_recovered[i] == P[i]) ? "CORRECT" : "CORRUPTED");
    }
    printf("\nAnswer to (a): Are any blocks beyond P2 affected?\n");
    printf("  -> NO. Blocks P3, P4, ... are completely UNAFFECTED (self-synchronizing property).\n");
    printf("     In CBC, P_i = Dec(C_i) ^ C_{i-1}. Only P1 (through Dec(C1)) and P2 (through C1)\n");
    printf("     depend on C1. P3 depends only on C3 and C2, which are uncorrupted.\n\n");

    printf("============================================================\n");
    printf("SCENARIO (b): Source Bit Error in Plaintext P1\n");
    printf("Suppose a bit error occurs in the original message P1 before encryption: P1' = P1 ^ 0x01\n");
    uint8_t P_src_err[4] = {P[0] ^ 0x01, P[1], P[2], P[3]};
    uint8_t C_src_err[4];
    prev = iv;
    for (int i = 0; i < 4; i++) {
        C_src_err[i] = enc_block(P_src_err[i] ^ prev, key);
        prev = C_src_err[i];
    }
    printf("Resulting Ciphertext Blocks:\n");
    for (int i = 0; i < 4; i++) {
        printf("  C%d: Altered=0x%02X, Original=0x%02X -> %s\n",
               i + 1, C_src_err[i], C_cbc[i],
               (C_src_err[i] == C_cbc[i]) ? "UNCHANGED" : "CHANGED");
    }

    // Receiver decrypts
    uint8_t P_rcv_b[4];
    P_rcv_b[0] = dec_block(C_src_err[0], key) ^ iv;
    P_rcv_b[1] = dec_block(C_src_err[1], key) ^ C_src_err[0];
    P_rcv_b[2] = dec_block(C_src_err[2], key) ^ C_src_err[1];
    P_rcv_b[3] = dec_block(C_src_err[3], key) ^ C_src_err[2];

    printf("\nReceiver Decryption Result:\n");
    for (int i = 0; i < 4; i++) {
        printf("  P%d: Received=0x%02X, Expected Original=0x%02X -> %s\n",
               i + 1, P_rcv_b[i], P[i],
               (P_rcv_b[i] == P[i]) ? "CORRECT" : "CORRUPTED");
    }

    printf("\nAnswer to (b):\n");
    printf("  1. Through how many ciphertext blocks is this error propagated?\n");
    printf("     -> ALL subsequent ciphertext blocks (C1, C2, C3, ..., Cn) are completely changed,\n");
    printf("        because each CBC block feeds into the next.\n");
    printf("  2. What is the effect at the receiver?\n");
    printf("     -> At the receiver, because the transmitted ciphertext is received without transmission errors,\n");
    printf("        decryption inverts the chain cleanly! ONLY P1 will contain the original source bit error.\n");
    printf("        Blocks P2, P3, ..., Pn will all be decrypted 100%% correctly without any corruption!\n");
    printf("============================================================\n");
    return 0;
}
