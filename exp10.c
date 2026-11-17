#include <stdio.h>
#include <string.h>
#include <ctype.h>

void findPos(char matrix[5][5], char ch, int *r, int *c) {
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
    char clean[512];
    int clen = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            if (ch == 'J') ch = 'I';
            clean[clen++] = ch;
        }
    }
    clean[clen] = '\0';

    int flen = 0;
    for (int i = 0; i < clen; i++) {
        output[flen++] = clean[i];
        if (i + 1 < clen) {
            if (clean[i] == clean[i + 1]) {
                output[flen++] = 'X';
            } else {
                output[flen++] = clean[++i];
            }
        }
    }
    if (flen % 2 != 0) {
        output[flen++] = 'X';
    }
    output[flen] = '\0';
}

void encryptPlayfair(char matrix[5][5], const char *input, char *cipher) {
    int len = strlen(input);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPos(matrix, input[i], &r1, &c1);
        findPos(matrix, input[i + 1], &r2, &c2);

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

int main() {
    char matrix[5][5] = {
        {'M', 'F', 'H', 'I', 'K'},
        {'U', 'N', 'O', 'P', 'Q'},
        {'Z', 'V', 'W', 'X', 'Y'},
        {'E', 'L', 'A', 'R', 'G'},
        {'D', 'S', 'T', 'B', 'C'}
    };

    const char *message = "Must see you over Cadogan West. Coming at once.";
    char prepared[512];
    char ciphertext[512];

    printf("============================================================\n");
    printf(" Experiment 10: Playfair Encryption with Specified Matrix   \n");
    printf("============================================================\n\n");

    printf("Given Playfair 5x5 Matrix:\n");
    for (int i = 0; i < 5; i++) {
        printf("  ");
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == 'I') printf("I/J ");
            else printf(" %c  ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nMessage to Encrypt:\n  \"%s\"\n\n", message);

    prepareText(message, prepared);
    printf("Prepared Digraphs (with 'X' fillers & padding):\n  ");
    for (int i = 0; prepared[i]; i += 2) {
        printf("%c%c ", prepared[i], prepared[i+1]);
    }
    printf("\n\n");

    encryptPlayfair(matrix, prepared, ciphertext);
    printf("Encrypted Ciphertext:\n  ");
    for (int i = 0; ciphertext[i]; i += 2) {
        printf("%c%c ", ciphertext[i], ciphertext[i+1]);
    }
    printf("\n============================================================\n");
    return 0;
}
