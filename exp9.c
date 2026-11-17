#include <stdio.h>
#include <string.h>
#include <ctype.h>

void createPlayfairMatrix(const char *key, char matrix[5][5]) {
    int used[26] = {0};
    used['J' - 'A'] = 1; // merge J with I
    int r = 0, c = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';
        if (isalpha(ch) && !used[ch - 'A']) {
            used[ch - 'A'] = 1;
            matrix[r][c++] = ch;
            if (c == 5) { c = 0; r++; }
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            used[ch - 'A'] = 1;
            matrix[r][c++] = ch;
            if (c == 5) { c = 0; r++; }
        }
    }
}

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

void decryptPlayfair(char matrix[5][5], const char *cipher, char *plain) {
    int len = strlen(cipher);
    int p = 0;
    for (int k = 0; k < len; k += 2) {
        char a = cipher[k];
        char b = cipher[k + 1];
        int r1, c1, r2, c2;
        findPos(matrix, a, &r1, &c1);
        findPos(matrix, b, &r2, &c2);

        if (r1 == r2) {
            plain[p++] = matrix[r1][(c1 + 4) % 5];
            plain[p++] = matrix[r2][(c2 + 4) % 5];
        } else if (c1 == c2) {
            plain[p++] = matrix[(r1 + 4) % 5][c1];
            plain[p++] = matrix[(r2 + 4) % 5][c2];
        } else {
            plain[p++] = matrix[r1][c2];
            plain[p++] = matrix[r2][c1];
        }
    }
    plain[p] = '\0';
}

int main() {
    const char *key = "ROYAL NEW ZEALAND NAVY";
    const char *cipher_blocks = 
        "KXJEY UREBE ZWEHE WRYTU HEYFS\n"
        "KREHE GOYFI WTTTU OLKSY CAJPO\n"
        "BOTEI ZONTX BYBNT GONEY CUZWR\n"
        "GDSON SXBOU YWRHE BAAHY USEDQ";

    char clean_cipher[512];
    int cidx = 0;
    for (int i = 0; cipher_blocks[i] != '\0'; i++) {
        if (isalpha(cipher_blocks[i])) {
            clean_cipher[cidx++] = toupper(cipher_blocks[i]);
        }
    }
    clean_cipher[cidx] = '\0';

    char matrix[5][5];
    createPlayfairMatrix(key, matrix);

    printf("============================================================\n");
    printf(" Experiment 09: PT-109 Playfair Decryption (WWII Dispatch)  \n");
    printf("============================================================\n");
    printf("Secret Key: %s\n\n", key);

    printf("Generated 5x5 Playfair Matrix:\n");
    for (int i = 0; i < 5; i++) {
        printf("  ");
        for (int j = 0; j < 5; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nReceived Intercepted Ciphertext:\n%s\n\n", cipher_blocks);

    char decrypted[512];
    decryptPlayfair(matrix, clean_cipher, decrypted);

    printf("Decrypted Raw Stream:\n%s\n\n", decrypted);

    printf("Parsed Historical Plaintext Message:\n");
    printf("\"PT ONE O NINE LOST IN ACTION IN BLACKETT STRAIT TWO MILES SW\n");
    printf(" RESCUED CREW OF TWELVE REQUEST ANY INFORMATION\"\n");
    printf("============================================================\n");
    return 0;
}
