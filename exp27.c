#include <stdio.h>
#include <string.h>
#include <ctype.h>

long long modExp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 27: Codebook Attack on Character-by-Character RSA\n");
    printf("============================================================\n\n");

    // Public Key (large modulus n, public exponent e)
    long long p = 3557, q = 2579;
    long long n = p * q; // 9173503 (large enough that factoring takes effort)
    long long e = 65537;

    printf("Public Parameters Known to All (including Attacker):\n");
    printf("  Modulus n = %lld\n", n);
    printf("  Exponent e = %lld\n\n", e);

    printf("Protocol Flaw:\n");
    printf("  Alice encrypts each English letter independently: A=0, B=1, ..., Z=25.\n");
    printf("  Question: Is this method secure?\n");
    printf("  Answer  : NO! It is trivially vulnerable to a Precomputed Codebook Attack.\n\n");

    // Attacker's Precomputation: Encrypt all 26 possible letters
    printf("Step 1: Attacker Precomputes Dictionary of all 26 possible ciphertexts:\n");
    printf("----------------------------------------------------------------------\n");
    long long codebook[26];
    for (int m = 0; m < 26; m++) {
        codebook[m] = modExp(m, e, n);
    }
    for (int m = 0; m < 26; m++) {
        printf("  '%c' (%2d) -> C = %-8lld", 'A' + m, m, codebook[m]);
        if ((m + 1) % 3 == 0) printf("\n");
    }
    printf("\n  Total effort required: exactly 26 modular exponentiations!\n\n");

    // Alice sends encrypted message
    const char *secret_msg = "CONFIDENTIAL";
    int msg_len = strlen(secret_msg);
    long long transmitted_ciphertext[64];

    printf("Step 2: Alice Transmits Encrypted Ciphertext Blocks:\n");
    printf("  Original message: \"%s\"\n", secret_msg);
    printf("  Ciphertext blocks sent over network:\n  ");
    for (int i = 0; i < msg_len; i++) {
        int m = secret_msg[i] - 'A';
        transmitted_ciphertext[i] = modExp(m, e, n);
        printf("%lld ", transmitted_ciphertext[i]);
    }
    printf("\n\n");

    // Step 3: Eve performs instant lookup attack
    printf("Step 3: Attacker (Eve) Intercepts and Decrypts by Table Lookup:\n");
    printf("----------------------------------------------------------------------\n");
    char intercepted_plaintext[64];
    for (int i = 0; i < msg_len; i++) {
        long long c = transmitted_ciphertext[i];
        char found = '?';
        for (int m = 0; m < 26; m++) {
            if (codebook[m] == c) {
                found = 'A' + m;
                break;
            }
        }
        intercepted_plaintext[i] = found;
        printf("  Block %2d: C = %-8lld -> Lookup matches '%c'\n", i + 1, c, found);
    }
    intercepted_plaintext[msg_len] = '\0';

    printf("\nResult of Attack:\n");
    printf("  Cracked Plaintext: \"%s\"\n", intercepted_plaintext);
    printf("  Matches Original : %s\n\n", (strcmp(intercepted_plaintext, secret_msg) == 0) ? "YES (100% Match!)" : "NO");

    printf("Remedy:\n");
    printf("  Never encrypt raw, small plaintext values without randomized padding (e.g. OAEP).\n");
    printf("============================================================\n");
    return 0;
}
