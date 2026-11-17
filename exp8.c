#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generateKeySequence(const char *keyword, char cipherSeq[27]) {
    int used[26] = {0};
    int idx = 0;

    for (int i = 0; keyword[i] != '\0'; i++) {
        char ch = toupper(keyword[i]);
        if (isalpha(ch) && !used[ch - 'A']) {
            used[ch - 'A'] = 1;
            cipherSeq[idx++] = ch;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!used[ch - 'A']) {
            used[ch - 'A'] = 1;
            cipherSeq[idx++] = ch;
        }
    }
    cipherSeq[26] = '\0';
}

void encryptMonoKeyword(const char *plain, const char *cipherSeq, char *cipher) {
    for (int i = 0; plain[i] != '\0'; i++) {
        if (isupper(plain[i])) {
            cipher[i] = cipherSeq[plain[i] - 'A'];
        } else if (islower(plain[i])) {
            cipher[i] = tolower(cipherSeq[plain[i] - 'a']);
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[strlen(plain)] = '\0';
}

void decryptMonoKeyword(const char *cipher, const char *cipherSeq, char *plain) {
    for (int i = 0; cipher[i] != '\0'; i++) {
        char ch = cipher[i];
        if (isupper(ch)) {
            char *p = strchr(cipherSeq, ch);
            plain[i] = 'A' + (p - cipherSeq);
        } else if (islower(ch)) {
            char *p = strchr(cipherSeq, toupper(ch));
            plain[i] = 'a' + (p - cipherSeq);
        } else {
            plain[i] = ch;
        }
    }
    plain[strlen(cipher)] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipherSeq[27];
    generateKeySequence(keyword, cipherSeq);

    char message[] = "Defend the east wall of the castle";
    char ciphertext[256];
    char decrypted[256];

    printf("============================================================\n");
    printf(" Experiment 08: Monoalphabetic Cipher with Keyword Sequence \n");
    printf("============================================================\n");
    printf("Keyword: %s\n\n", keyword);

    printf("Plain Alphabet : ");
    for (char c = 'a'; c <= 'z'; c++) printf("%c ", c);
    printf("\nCipher Sequence: ");
    for (int i = 0; i < 26; i++) printf("%c ", cipherSeq[i]);
    printf("\n\n");

    printf("Original Message : %s\n", message);

    encryptMonoKeyword(message, cipherSeq, ciphertext);
    printf("Encrypted Output : %s\n", ciphertext);

    decryptMonoKeyword(ciphertext, cipherSeq, decrypted);
    printf("Decrypted Output : %s\n", decrypted);
    printf("============================================================\n");
    return 0;
}
