#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encryptVigenere(const char *plain, const char *key, char *cipher) {
    int keyLen = strlen(key);
    int j = 0;
    for (int i = 0; plain[i] != '\0'; i++) {
        if (isalpha(plain[i])) {
            char base = isupper(plain[i]) ? 'A' : 'a';
            char k = toupper(key[j % keyLen]) - 'A';
            cipher[i] = (char)(((toupper(plain[i]) - 'A' + k) % 26) + base);
            j++;
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[strlen(plain)] = '\0';
}

void decryptVigenere(const char *cipher, const char *key, char *plain) {
    int keyLen = strlen(key);
    int j = 0;
    for (int i = 0; cipher[i] != '\0'; i++) {
        if (isalpha(cipher[i])) {
            char base = isupper(cipher[i]) ? 'A' : 'a';
            char k = toupper(key[j % keyLen]) - 'A';
            plain[i] = (char)(((toupper(cipher[i]) - 'A' - k + 26) % 26) + base);
            j++;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[strlen(cipher)] = '\0';
}

int main() {
    char plaintext[] = "GEEKSFORGEEKS";
    char key[] = "AYUSH";
    char ciphertext[256];
    char decrypted[256];

    printf("========================================\n");
    printf(" Experiment 04: Polyalphabetic (Vigenere)\n");
    printf("========================================\n");
    printf("Plaintext : %s\n", plaintext);
    printf("Key       : %s\n\n", key);

    encryptVigenere(plaintext, key, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);

    decryptVigenere(ciphertext, key, decrypted);
    printf("Decrypted : %s\n", decrypted);
    printf("========================================\n");
    return 0;
}
