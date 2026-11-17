#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Plain alphabet:  ABCDEFGHIJKLMNOPQRSTUVWXYZ
// Cipher alphabet: QWERTYUIOPASDFGHJKLZXCVBNM
const char *PLAIN_ALPHABET  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *CIPHER_ALPHABET = "QWERTYUIOPASDFGHJKLZXCVBNM";

void encryptMono(const char *plain, char *cipher) {
    for (int i = 0; plain[i] != '\0'; i++) {
        if (isupper(plain[i])) {
            cipher[i] = CIPHER_ALPHABET[plain[i] - 'A'];
        } else if (islower(plain[i])) {
            cipher[i] = tolower(CIPHER_ALPHABET[plain[i] - 'a']);
        } else {
            cipher[i] = plain[i];
        }
    }
    cipher[strlen(plain)] = '\0';
}

void decryptMono(const char *cipher, char *plain) {
    for (int i = 0; cipher[i] != '\0'; i++) {
        char ch = cipher[i];
        if (isupper(ch)) {
            char *p = strchr(CIPHER_ALPHABET, ch);
            plain[i] = PLAIN_ALPHABET[p - CIPHER_ALPHABET];
        } else if (islower(ch)) {
            char *p = strchr(CIPHER_ALPHABET, toupper(ch));
            plain[i] = tolower(PLAIN_ALPHABET[p - CIPHER_ALPHABET]);
        } else {
            plain[i] = ch;
        }
    }
    plain[strlen(cipher)] = '\0';
}

int main() {
    char plaintext[256] = "ATTACK AT DAWN";
    char ciphertext[256];
    char decrypted[256];

    printf("========================================\n");
    printf(" Experiment 02: Monoalphabetic Cipher   \n");
    printf("========================================\n");
    printf("Plain Alphabet : %s\n", PLAIN_ALPHABET);
    printf("Cipher Alphabet: %s\n\n", CIPHER_ALPHABET);

    printf("Plaintext : %s\n", plaintext);
    encryptMono(plaintext, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);

    decryptMono(ciphertext, decrypted);
    printf("Decrypted : %s\n", decrypted);
    printf("========================================\n");
    return 0;
}
