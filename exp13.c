#include <stdio.h>
#include <string.h>

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 13: Known & Chosen Plaintext Attacks on Hill Cipher\n");
    printf("============================================================\n\n");

    // Secret Key Matrix K (2x2) unknown to attacker:
    int secret_K[2][2] = {
        {9, 4},
        {5, 7}
    };

    printf("Secret Key Matrix K:\n  [ %2d  %2d ]\n  [ %2d  %2d ]\n\n",
           secret_K[0][0], secret_K[0][1], secret_K[1][0], secret_K[1][1]);

    printf("1. KNOWN PLAINTEXT ATTACK (KPA):\n");
    printf("------------------------------------------------------------\n");
    printf("Suppose an attacker intercepts two plaintext-ciphertext pairs:\n");
    printf("  Pair 1: P1 = [ 3,  2] (\"DC\")\n");
    printf("  Pair 2: P2 = [ 5,  7] (\"FH\")\n");

    int P[2][2] = {
        {3, 2},
        {5, 7}
    };

    // Corresponding ciphertext C = P * K mod 26
    int C[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            C[i][j] = (P[i][0] * secret_K[0][j] + P[i][1] * secret_K[1][j]) % 26;
        }
    }

    printf("  Resulting Intercepted Ciphertext Pairs:\n");
    printf("  Pair 1: C1 = [ %2d, %2d ]\n", C[0][0], C[0][1]);
    printf("  Pair 2: C2 = [ %2d, %2d ]\n\n", C[1][0], C[1][1]);

    // Matrix equation: C = P * K (mod 26)
    // To solve for K: K = P^(-1) * C (mod 26)
    int detP = (P[0][0] * P[1][1] - P[0][1] * P[1][0]) % 26;
    if (detP < 0) detP += 26;
    int detP_inv = modInverse(detP, 26);

    printf("Plaintext Matrix P:\n  [ %2d  %2d ]\n  [ %2d  %2d ]\n", P[0][0], P[0][1], P[1][0], P[1][1]);
    printf("det(P) = (3*7 - 2*5) mod 26 = 11 mod 26\n");
    printf("det(P)^(-1) mod 26 = %d (since 11 * %d = %d = 1 mod 26)\n\n", detP_inv, detP_inv, 11 * detP_inv);

    if (detP_inv != -1) {
        int P_inv[2][2];
        P_inv[0][0] = (detP_inv * P[1][1]) % 26;
        P_inv[0][1] = (detP_inv * (-P[0][1] + 26)) % 26;
        P_inv[1][0] = (detP_inv * (-P[1][0] + 26)) % 26;
        P_inv[1][1] = (detP_inv * P[0][0]) % 26;

        int recovered_K[2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                recovered_K[i][j] = (P_inv[i][0] * C[0][j] + P_inv[i][1] * C[1][j]) % 26;
            }
        }

        printf("Recovered Key Matrix K = P^(-1) * C mod 26:\n");
        printf("  [ %2d  %2d ]\n", recovered_K[0][0], recovered_K[0][1]);
        printf("  [ %2d  %2d ]\n", recovered_K[1][0], recovered_K[1][1]);
        printf("Matches Secret Key: %s\n\n",
               (recovered_K[0][0] == secret_K[0][0] && recovered_K[0][1] == secret_K[0][1] &&
                recovered_K[1][0] == secret_K[1][0] && recovered_K[1][1] == secret_K[1][1]) ? "YES (SUCCESSFULLY CRACKED!)" : "NO");
    }

    printf("2. CHOSEN PLAINTEXT ATTACK (CPA):\n");
    printf("------------------------------------------------------------\n");
    printf("Attacker chooses plaintext P to be the identity matrix I:\n");
    printf("  Block 1: (1, 0) -> 'BA'\n");
    printf("  Block 2: (0, 1) -> 'AB'\n");
    printf("Since C = I * K = K mod 26:\n");
    int cpa_C[2][2];
    cpa_C[0][0] = secret_K[0][0]; cpa_C[0][1] = secret_K[0][1];
    cpa_C[1][0] = secret_K[1][0]; cpa_C[1][1] = secret_K[1][1];
    printf("Ciphertext directly reveals Key K:\n");
    printf("  [ %2d  %2d ]\n", cpa_C[0][0], cpa_C[0][1]);
    printf("  [ %2d  %2d ]\n", cpa_C[1][0], cpa_C[1][1]);
    printf("CPA complexity is trivial: exactly 2 chosen blocks reveal entire key matrix!\n");
    printf("============================================================\n");
    return 0;
}
