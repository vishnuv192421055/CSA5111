#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Mock block cipher encryption function: E_K(block)
uint64_t block_cipher_encrypt(uint64_t block, uint64_t key) {
    // Standard 64-bit reversible Feistel-like mixing
    uint32_t L = (uint32_t)(block >> 32);
    uint32_t R = (uint32_t)(block & 0xFFFFFFFFULL);
    uint32_t kL = (uint32_t)(key >> 32);
    uint32_t kR = (uint32_t)(key & 0xFFFFFFFFULL);

    for (int round = 0; round < 4; round++) {
        uint32_t temp = R;
        R = L ^ (((R << 5) | (R >> 27)) + kL + 0x9E3779B9);
        L = temp;
    }
    return ((uint64_t)L << 32) | R;
}

// Standard CBC-MAC computation for n blocks (IV = 0)
uint64_t compute_cbc_mac(const uint64_t *blocks, int n, uint64_t key) {
    uint64_t mac = 0; // IV = 0
    for (int i = 0; i < n; i++) {
        mac = block_cipher_encrypt(blocks[i] ^ mac, key);
    }
    return mac;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 30: CBC-MAC Length Extension Forgery Attack     \n");
    printf("============================================================\n\n");

    uint64_t secret_key = 0x0123456789ABCDEFULL;

    // 1-block message X
    uint64_t X = 0xA1B2C3D4E5F60718ULL;

    printf("Step 1: Honest Sender Computes CBC-MAC for 1-Block Message X:\n");
    printf("  Message Block X : 0x%016llX\n", (unsigned long long)X);
    uint64_t T = compute_cbc_mac(&X, 1, secret_key);
    printf("  Tag T = MAC(K, X): 0x%016llX\n\n", (unsigned long long)T);

    printf("Step 2: Adversary Crafts 2-Block Forgery WITHOUT knowing Key K:\n");
    printf("  Adversary forms message M = X || (X ^ T)\n");
    uint64_t Y = X ^ T;
    printf("  Block 1 (X)     : 0x%016llX\n", (unsigned long long)X);
    printf("  Block 2 (X ^ T) : 0x%016llX ^ 0x%016llX = 0x%016llX\n\n",
           (unsigned long long)X, (unsigned long long)T, (unsigned long long)Y);

    printf("Step 3: Verification of the Forgery:\n");
    printf("  Mathematical Proof:\n");
    printf("    C1 = E_K(X ^ 0) = E_K(X) = T\n");
    printf("    C2 = E_K(Block2 ^ C1) = E_K((X ^ T) ^ T) = E_K(X ^ (T ^ T)) = E_K(X) = T\n\n");

    uint64_t forged_message[2] = {X, Y};
    uint64_t forged_T = compute_cbc_mac(forged_message, 2, secret_key);

    printf("  Actual Tag computed for 2-block message: 0x%016llX\n", (unsigned long long)forged_T);
    printf("  Original 1-block Tag T                 : 0x%016llX\n", (unsigned long long)T);
    printf("  Tags Match exactly: %s\n\n", (forged_T == T) ? "YES (FORGERY SUCCEEDED!)" : "NO");

    printf("Conclusion & Security Recommendation:\n");
    printf("  Basic CBC-MAC is NOT secure for variable-length messages.\n");
    printf("  CMAC (NIST SP 800-38B) fixes this vulnerability by XORing distinct subkeys\n");
    printf("  (K1 or K2) into the final block before encryption.\n");
    printf("============================================================\n");
    return 0;
}
