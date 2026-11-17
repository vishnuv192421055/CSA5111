#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    printf("============================================================\n");
    printf(" Experiment 35: One-Time Pad Vigenere Cipher Stream Attack  \n");
    printf("============================================================\n\n");

    const char *p1_str = "sendmoremoney";
    int key1[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int len = 13;

    // (a) Encrypt
    char cipher[16];
    printf("(a) Encryption of \"send more money\":\n");
    printf("    Plaintext : %s\n", p1_str);
    printf("    Key Stream: ");
    for (int i = 0; i < len; i++) printf("%2d ", key1[i]);
    printf("\n    Ciphertext: ");
    for (int i = 0; i < len; i++) {
        int c_val = ((p1_str[i] - 'a') + key1[i]) % 26;
        cipher[i] = 'a' + c_val;
        printf(" %c ", cipher[i]);
    }
    cipher[len] = '\0';
    printf(" (\"%s\")\n\n", cipher);

    // (b) Find key for "cash not needed"
    const char *p2_str = "cashnotneeded";
    int key2[16];
    printf("(b) Finding Key Stream to Decrypt to \"cash not needed\":\n");
    printf("    Target Plaintext: %s\n", p2_str);
    printf("    Formula: Key[i] = (Cipher[i] - Target[i] + 26) mod 26\n");
    printf("    Derived Key     : ");
    for (int i = 0; i < len; i++) {
        key2[i] = ((cipher[i] - 'a') - (p2_str[i] - 'a') + 26) % 26;
        printf("%2d ", key2[i]);
    }
    printf("\n\n");

    printf("Decryption Verification with Derived Key:\n");
    printf("    Decrypted: ");
    for (int i = 0; i < len; i++) {
        int dec = ((cipher[i] - 'a') - key2[i] + 26) % 26;
        printf("%c", 'a' + dec);
    }
    printf(" (Successfully matched!)\n");
    printf("============================================================\n");
    return 0;
}
