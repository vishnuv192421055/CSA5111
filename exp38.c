#include <stdio.h>

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 38: Known & Chosen Plaintext Attack on Hill Cipher\n");
    printf("============================================================\n\n");

    // Hill Cipher 2x2: C = P * K mod 26
    int secret_K[2][2] = {
        {3, 5},
        {6, 17}
    };

    printf("Secret Key Matrix K:\n  [ %2d  %2d ]\n  [ %2d  %2d ]\n\n",
           secret_K[0][0], secret_K[0][1], secret_K[1][0], secret_K[1][1]);

    // Intercepted Known Plaintext-Ciphertext Pairs
    // P = [3, 2; 5, 7] -> det(P) = 11 coprime to 26
    int P[2][2] = {{3, 2}, {5, 7}};
    int C[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            C[i][j] = (P[i][0] * secret_K[0][j] + P[i][1] * secret_K[1][j]) % 26;
        }
    }

    printf("1. Intercepted Known Plaintext-Ciphertext Pairs:\n");
    printf("   P1 = [%2d, %2d] -> C1 = [%2d, %2d]\n", P[0][0], P[0][1], C[0][0], C[0][1]);
    printf("   P2 = [%2d, %2d] -> C2 = [%2d, %2d]\n\n", P[1][0], P[1][1], C[1][0], C[1][1]);

    int detP = (P[0][0] * P[1][1] - P[0][1] * P[1][0]) % 26;
    if (detP < 0) detP += 26;
    int detP_inv = modInverse(detP, 26);

    printf("   det(P) = %d, det(P)^(-1) mod 26 = %d\n", detP, detP_inv);

    if (detP_inv != -1) {
        int P_inv[2][2];
        P_inv[0][0] = (detP_inv * P[1][1]) % 26;
        P_inv[0][1] = (detP_inv * (-P[0][1] + 26)) % 26;
        P_inv[1][0] = (detP_inv * (-P[1][0] + 26)) % 26;
        P_inv[1][1] = (detP_inv * P[0][0]) % 26;

        int rec_K[2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                rec_K[i][j] = (P_inv[i][0] * C[0][j] + P_inv[i][1] * C[1][j]) % 26;
            }
        }
        printf("\n   Recovered Key K = P^(-1) * C mod 26:\n");
        printf("     [ %2d  %2d ]\n     [ %2d  %2d ]\n",
               rec_K[0][0], rec_K[0][1], rec_K[1][0], rec_K[1][1]);
        printf("   Match Status: %s\n\n",
               (rec_K[0][0] == secret_K[0][0] && rec_K[0][1] == secret_K[0][1] &&
                rec_K[1][0] == secret_K[1][0] && rec_K[1][1] == secret_K[1][1]) ? "SUCCESSFUL (Exact Key Recovered!)" : "FAILED");
    }

    printf("2. Chosen Plaintext Attack:\n");
    printf("   By choosing P = Identity Matrix [1 0; 0 1], C = I * K = K.\n");
    printf("   Key is directly read from the ciphertext with zero inversion math!\n");
    printf("============================================================\n");
    return 0;
}
