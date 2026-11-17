#include <stdio.h>
#include <string.h>
#include <ctype.h>

void createMatrix(const char *key, char matrix[5][5]) {
    int map[26] = {0};
    map['J' - 'A'] = 1; // treat I and J as same
    int r = 0, c = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';
        if (ch >= 'A' && ch <= 'Z' && !map[ch - 'A']) {
            map[ch - 'A'] = 1;
            matrix[r][c++] = ch;
            if (c == 5) { c = 0; r++; }
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!map[ch - 'A']) {
            map[ch - 'A'] = 1;
            matrix[r][c++] = ch;
            if (c == 5) { c = 0; r++; }
        }
    }
}

void findPosition(char matrix[5][5], char ch, int *r, int *c) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == ch) {
                *r = i;
                *c = j;
                return;
            }
        }
    }
}

void prepareText(const char *input, char *output) {
    int len = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            if (ch == 'J') ch = 'I';
            output[len++] = ch;
        }
    }
    output[len] = '\0';

    char formatted[512];
    int flen = 0;
    for (int i = 0; i < len; i++) {
        formatted[flen++] = output[i];
        if (i + 1 < len) {
            if (output[i] == output[i + 1]) {
                formatted[flen++] = 'X';
            } else {
                formatted[flen++] = output[++i];
            }
        }
    }
    if (flen % 2 != 0) {
        formatted[flen++] = 'X';
    }
    formatted[flen] = '\0';
    strcpy(output, formatted);
}

void encryptPlayfair(char matrix[5][5], const char *input, char *cipher) {
    int len = strlen(input);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, input[i], &r1, &c1);
        findPosition(matrix, input[i + 1], &r2, &c2);

        if (r1 == r2) {
            cipher[i]     = matrix[r1][(c1 + 1) % 5];
            cipher[i + 1] = matrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) {
            cipher[i]     = matrix[(r1 + 1) % 5][c1];
            cipher[i + 1] = matrix[(r2 + 1) % 5][c2];
        } else {
            cipher[i]     = matrix[r1][c2];
            cipher[i + 1] = matrix[r2][c1];
        }
    }
    cipher[len] = '\0';
}

void decryptPlayfair(char matrix[5][5], const char *cipher, char *plain) {
    int len = strlen(cipher);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, cipher[i], &r1, &c1);
        findPosition(matrix, cipher[i + 1], &r2, &c2);

        if (r1 == r2) {
            plain[i]     = matrix[r1][(c1 + 4) % 5];
            plain[i + 1] = matrix[r2][(c2 + 4) % 5];
        } else if (c1 == c2) {
            plain[i]     = matrix[(r1 + 4) % 5][c1];
            plain[i + 1] = matrix[(r2 + 4) % 5][c2];
        } else {
            plain[i]     = matrix[r1][c2];
            plain[i + 1] = matrix[r2][c1];
        }
    }
    plain[len] = '\0';
}

int main() {
    char key[] = "MONARCHY";
    char plaintext[] = "INSTRUMENTS";
    char matrix[5][5];
    char prepared[512];
    char ciphertext[512];
    char decrypted[512];

    printf("========================================\n");
    printf(" Experiment 03: Playfair Cipher         \n");
    printf("========================================\n");
    printf("Key: %s\n", key);

    createMatrix(key, matrix);
    printf("\nPlayfair 5x5 Matrix:\n");
    for (int i = 0; i < 5; i++) {
        printf("  ");
        for (int j = 0; j < 5; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    prepareText(plaintext, prepared);
    printf("\nOriginal Plaintext: %s\n", plaintext);
    printf("Prepared Digraphs : %s\n", prepared);

    encryptPlayfair(matrix, prepared, ciphertext);
    printf("Ciphertext        : %s\n", ciphertext);

    decryptPlayfair(matrix, ciphertext, decrypted);
    printf("Decrypted Plaintext: %s\n", decrypted);
    printf("========================================\n");
    return 0;
}
