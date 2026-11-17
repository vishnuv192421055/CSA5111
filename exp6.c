#include <stdio.h>
#include <string.h>
#include <ctype.h>

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 06: Breaking Affine Cipher with Frequency Analysis\n");
    printf("============================================================\n\n");

    printf("Given Information:\n");
    printf("  1. Most frequent ciphertext letter   : 'B' (numerical value = 1)\n");
    printf("  2. Second most frequent cipher letter: 'U' (numerical value = 20)\n\n");
    printf("Standard English Frequency Knowledge:\n");
    printf("  1. Most frequent English letter      : 'E' (numerical value = 4)\n");
    printf("  2. Second most frequent English letter: 'T' (numerical value = 19)\n\n");

    printf("Setting up Affine equations: C = (a * p + b) mod 26\n");
    printf("  Equation 1 (for E -> B): (4*a + b) = 1  (mod 26)\n");
    printf("  Equation 2 (for T -> U): (19*a + b) = 20 (mod 26)\n\n");

    printf("Subtracting Eq 1 from Eq 2:\n");
    printf("  (19 - 4)*a = (20 - 1) (mod 26)\n");
    printf("  15*a = 19 (mod 26)\n\n");

    int diff_p = (19 - 4 + 26) % 26; // 15
    int diff_c = (20 - 1 + 26) % 26; // 19
    int inv = modInverse(diff_p, 26);

    printf("Multiplicative inverse of %d mod 26 is: %d  (since %d * %d = %d = %d*26 + 1)\n",
           diff_p, inv, diff_p, inv, diff_p * inv, (diff_p * inv) / 26);

    int a = (diff_c * inv) % 26;
    printf("  a = (19 * %d) mod 26 = %d mod 26 = %d\n", inv, diff_c * inv, a);

    // b = (1 - 4*a) mod 26
    int b = (1 - 4 * a) % 26;
    if (b < 0) b += 26;
    printf("Substituting 'a' back into Eq 1:\n");
    printf("  b = (1 - 4 * %d) mod 26 = (%d) mod 26 = %d\n\n", a, 1 - 4 * a, b);

    printf("--> Recovered Key: a = %d, b = %d\n", a, b);

    int a_inv = modInverse(a, 26);
    printf("--> Decryption Key: a^(-1) = %d, b = %d\n", a_inv, b);
    printf("    Formula: p = %d * (C - %d) mod 26\n\n", a_inv, b);

    // Verification
    printf("Verification:\n");
    int test_B = (a_inv * (1 - b + 26)) % 26;
    int test_U = (a_inv * (20 - b + 26)) % 26;
    printf("  Decrypt('B' = 1)  -> %2d ('%c')  [Matches 'E']\n", test_B, 'A' + test_B);
    printf("  Decrypt('U' = 20) -> %2d ('%c')  [Matches 'T']\n\n", test_U, 'A' + test_U);

    // Sample decryption
    char ciphertext[] = "B U B U";
    printf("Sample Ciphertext: \"%s\"\n", ciphertext);
    printf("Decrypted Output : ");
    for (int i = 0; ciphertext[i]; i++) {
        if (isupper(ciphertext[i])) {
            int c = ciphertext[i] - 'A';
            int p = (a_inv * (c - b + 260)) % 26;
            printf("%c", 'A' + p);
        } else {
            printf("%c", ciphertext[i]);
        }
    }
    printf("\n============================================================\n");
    return 0;
}
