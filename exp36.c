#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int t = b; b = a % b; a = t;
    }
    return a;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 36: Affine Caesar Cipher Validity & Key Analysis\n");
    printf(" Formula: C = E([a, b], p) = (a * p + b) mod 26             \n");
    printf("============================================================\n\n");

    printf("(a) Limitations on the value of 'b':\n");
    printf("    Any value of b in {0, 1, ..., 25} is mathematically valid.\n");
    printf("    'b' is an additive translation and does not alter the bijectivity of the cipher.\n\n");

    printf("(b) Values of 'a' that are NOT ALLOWED:\n");
    printf("    For the cipher to be invertible (one-to-one), gcd(a, 26) must be 1.\n");
    printf("    Any value of 'a' that shares a factor with 26 (factors 2 and 13) is NOT allowed.\n\n");

    printf("List of Disallowed Values of 'a':\n  ");
    int count = 0;
    for (int a = 0; a < 26; a++) {
        if (gcd(a, 26) != 1) {
            printf("%2d ", a);
            count++;
        }
    }
    printf("\n  Total disallowed values = %d (all 13 even numbers + 13)\n\n", count);

    printf("Example Collision with Disallowed a = 2, b = 3:\n");
    int a = 2, b = 3;
    printf("  Plaintext 'A' (p = 0)  -> C = (2 * 0 + 3) mod 26 = %d ('D')\n", (a * 0 + b) % 26);
    printf("  Plaintext 'N' (p = 13) -> C = (2 * 13 + 3) mod 26 = %d ('D')\n", (a * 13 + b) % 26);
    printf("  Two distinct plaintexts yield identical ciphertext. Decryption impossible!\n");
    printf("============================================================\n");
    return 0;
}
