#include <stdio.h>
#include <string.h>
#include <ctype.h>

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 12: Hill Cipher (2x2 Matrix Encryption/Decryption)\n");
    printf("============================================================\n\n");

    int K[2][2] = {
        {9, 4},
        {5, 7}
    };

    printf("Key Matrix K:\n");
    printf("  [ %2d  %2d ]\n", K[0][0], K[0][1]);
    printf("  [ %2d  %2d ]\n\n", K[1][0], K[1][1]);

    // Determinant and inverse
    int det = (K[0][0] * K[1][1] - K[0][1] * K[1][0]) % 26;
    if (det < 0) det += 26;
    int det_inv = modInverse(det, 26);

    printf("Determinant = (9*7 - 4*5) mod 26 = 43 mod 26 = %d\n", det);
    printf("det^(-1) mod 26 = %d (since %d * %d = %d = 1 mod 26)\n\n",
           det_inv, det, det_inv, det * det_inv);

    // K inverse matrix
    int K_inv[2][2];
    K_inv[0][0] = (det_inv * K[1][1]) % 26;
    K_inv[0][1] = (det_inv * (-K[0][1] + 26)) % 26;
    K_inv[1][0] = (det_inv * (-K[1][0] + 26)) % 26;
    K_inv[1][1] = (det_inv * K[0][0]) % 26;

    printf("Inverse Key Matrix K^(-1) mod 26:\n");
    printf("  [ %2d  %2d ]\n", K_inv[0][0], K_inv[0][1]);
    printf("  [ %2d  %2d ]\n\n", K_inv[1][0], K_inv[1][1]);

    const char *raw_msg = "meet me at the usual place at ten rather than eight oclock";
    char plain[512];
    int plen = 0;
    for (int i = 0; raw_msg[i]; i++) {
        if (isalpha(raw_msg[i])) {
            plain[plen++] = toupper(raw_msg[i]);
        }
    }
    if (plen % 2 != 0) {
        plain[plen++] = 'X'; // padding if odd
    }
    plain[plen] = '\0';

    printf("Prepared Plaintext (%d letters):\n  %s\n\n", plen, plain);

    char cipher[512];
    printf("(a) Encryption Calculations (C = P * K mod 26):\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < plen; i += 2) {
        int p1 = plain[i] - 'A';
        int p2 = plain[i + 1] - 'A';
        int c1 = (p1 * K[0][0] + p2 * K[1][0]) % 26;
        int c2 = (p1 * K[0][1] + p2 * K[1][1]) % 26;
        cipher[i] = 'A' + c1;
        cipher[i + 1] = 'A' + c2;
        if (i < 8) { // print first 4 blocks calculation
            printf("  Pair [%c,%c] = [%2d,%2d] -> [%2d*9 + %2d*5, %2d*4 + %2d*7] mod 26 = [%2d,%2d] -> [%c,%c]\n",
                   plain[i], plain[i+1], p1, p2, p1, p2, p1, p2, c1, c2, cipher[i], cipher[i+1]);
        }
    }
    cipher[plen] = '\0';
    printf("  ... (all %d pairs calculated similarly)\n\n", plen / 2);
    printf("Final Ciphertext:\n  %s\n\n", cipher);

    char decrypted[512];
    printf("(b) Decryption Calculations (P = C * K^(-1) mod 26):\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < plen; i += 2) {
        int c1 = cipher[i] - 'A';
        int c2 = cipher[i + 1] - 'A';
        int p1 = (c1 * K_inv[0][0] + c2 * K_inv[1][0]) % 26;
        int p2 = (c1 * K_inv[0][1] + c2 * K_inv[1][1]) % 26;
        decrypted[i] = 'A' + p1;
        decrypted[i + 1] = 'A' + p2;
        if (i < 8) {
            printf("  Pair [%c,%c] = [%2d,%2d] -> [%2d*5 + %2d*15, %2d*12 + %2d*25] mod 26 = [%2d,%2d] -> [%c,%c]\n",
                   cipher[i], cipher[i+1], c1, c2, c1, c2, c1, c2, p1, p2, decrypted[i], decrypted[i+1]);
        }
    }
    decrypted[plen] = '\0';
    printf("  ... (all pairs decrypted)\n\n");
    printf("Recovered Original Plaintext:\n  %s\n", decrypted);
    printf("============================================================\n");
    return 0;
}
