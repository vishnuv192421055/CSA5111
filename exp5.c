#include <stdio.h>
#include <stdbool.h>

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
    printf(" Experiment 05: Affine Caesar Cipher Validity & Constraints \n");
    printf(" Formula: C = E([a, b], p) = (a*p + b) mod 26               \n");
    printf("============================================================\n\n");

    printf("(a) Limitations on the value of b:\n");
    printf("    There are NO mathematical restrictions on 'b' for invertibility.\n");
    printf("    'b' acts as an additive shift (translation). Any integer b in {0, 1, ..., 25}\n");
    printf("    is valid and preserves the one-to-one mapping.\n\n");

    printf("(b) Limitations on the value of 'a':\n");
    printf("    For the cipher to be one-to-one (bijective), 'a' must be coprime to 26,\n");
    printf("    i.e., gcd(a, 26) == 1. This ensures that 'a' has a modular inverse mod 26.\n\n");

    printf("Analysis of all values of a in {0, 1, ..., 25}:\n");
    printf("------------------------------------------------------------\n");
    printf(" a  | gcd(a,26) | Status       | Modular Inverse (mod 26)\n");
    printf("----+-----------+--------------+-------------------------\n");
    int allowedCount = 0, notAllowedCount = 0;
    for (int a = 0; a < 26; a++) {
        int g = gcd(a, 26);
        if (g == 1) {
            printf(" %2d |     %d     | ALLOWED      | a^(-1) = %2d\n", a, g, modInverse(a, 26));
            allowedCount++;
        } else {
            printf(" %2d |     %d     | NOT ALLOWED  | No inverse\n", a, g);
            notAllowedCount++;
        }
    }
    printf("------------------------------------------------------------\n");
    printf("Total Allowed values of a     : %d\n", allowedCount);
    printf("Total Disallowed values of a  : %d (All even numbers and 13)\n\n", notAllowedCount);

    printf("Demonstration of Failure when gcd(a, 26) != 1:\n");
    printf("Example given: a = 2, b = 3\n");
    int a_bad = 2, b_bad = 3;
    int p1 = 0, p2 = 13;
    int c1 = (a_bad * p1 + b_bad) % 26;
    int c2 = (a_bad * p2 + b_bad) % 26;
    printf("  For p = %2d ('A'): C = (2 * %2d + 3) mod 26 = %d ('%c')\n", p1, p1, c1, 'A' + c1);
    printf("  For p = %2d ('N'): C = (2 * %2d + 3) mod 26 = %d ('%c')\n", p2, p2, c2, 'A' + c2);
    printf("  Both p = 0 and p = 13 collide and map to C = 3 ('D'). Decryption is impossible!\n");
    printf("============================================================\n");
    return 0;
}
