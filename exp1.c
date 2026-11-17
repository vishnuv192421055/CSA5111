#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encryptCaesar(char text[], int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            text[i] = (char)(((text[i] - 'A' + k) % 26) + 'A');
        } else if (islower(text[i])) {
            text[i] = (char)(((text[i] - 'a' + k) % 26) + 'a');
        }
    }
}

void decryptCaesar(char text[], int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            text[i] = (char)(((text[i] - 'A' - k + 26) % 26) + 'A');
        } else if (islower(text[i])) {
            text[i] = (char)(((text[i] - 'a' - k + 26) % 26) + 'a');
        }
    }
}

int main() {
    char message[256] = "The Quick Brown Fox Jumps Over The Lazy Dog";
    int k = 3;

    printf("========================================\n");
    printf(" Experiment 01: Caesar Cipher (k = 1..25)\n");
    printf("========================================\n");
    printf("Plaintext  : %s\n", message);
    printf("Shift key k: %d\n\n", k);

    encryptCaesar(message, k);
    printf("Ciphertext : %s\n", message);

    decryptCaesar(message, k);
    printf("Decrypted  : %s\n", message);
    printf("========================================\n");
    return 0;
}
