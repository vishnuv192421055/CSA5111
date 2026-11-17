#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    printf("============================================================\n");
    printf(" Experiment 14: One-Time Pad (Vigenere Stream) Cipher       \n");
    printf("============================================================\n\n");

    // Part (a)
    const char *p1_raw = "send more money";
    int key1[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int len1 = 13;

    char p1[32];
    int idx = 0;
    for (int i = 0; p1_raw[i]; i++) {
        if (isalpha(p1_raw[i])) p1[idx++] = tolower(p1_raw[i]);
    }
    p1[idx] = '\0';

    printf("(a) Encrypting \"send more money\" with Key Stream:\n");
    printf("    Key Stream: ");
    for (int i = 0; i < len1; i++) printf("%2d ", key1[i]);
    printf("\n");

    printf("    Plaintext : ");
    for (int i = 0; i < len1; i++) printf(" %c ", p1[i]);
    printf("\n");

    printf("    Numeric P : ");
    for (int i = 0; i < len1; i++) printf("%2d ", p1[i] - 'a');
    printf("\n");

    char c1[32];
    printf("    Formula   : C[i] = (P[i] + K[i]) mod 26\n");
    printf("    Ciphertext: ");
    for (int i = 0; i < len1; i++) {
        int c_val = ((p1[i] - 'a') + key1[i]) % 26;
        c1[i] = 'a' + c_val;
        printf(" %c ", c1[i]);
    }
    c1[len1] = '\0';
    printf(" (Stream: \"%s\")\n\n", c1);

    // Part (b)
    // Find key2 such that c1 decrypts to "cash not needed"
    const char *p2_raw = "cash not needed";
    char p2[32];
    idx = 0;
    for (int i = 0; p2_raw[i]; i++) {
        if (isalpha(p2_raw[i])) p2[idx++] = tolower(p2_raw[i]);
    }
    p2[idx] = '\0';

    printf("(b) Finding New Key Stream so Ciphertext Decrypts to \"cash not needed\":\n");
    printf("    Target Plaintext: ");
    for (int i = 0; i < len1; i++) printf(" %c ", p2[i]);
    printf("\n");

    printf("    Numeric Target  : ");
    for (int i = 0; i < len1; i++) printf("%2d ", p2[i] - 'a');
    printf("\n");

    printf("    Using formula   : K2[i] = (C[i] - P2[i] + 26) mod 26\n");
    int key2[32];
    printf("    Required Key    : ");
    for (int i = 0; i < len1; i++) {
        key2[i] = ((c1[i] - 'a') - (p2[i] - 'a') + 26) % 26;
        printf("%2d ", key2[i]);
    }
    printf("\n\n");

    printf("Verification of Decryption with Key 2:\n");
    printf("    Decrypted: ");
    for (int i = 0; i < len1; i++) {
        int dec_val = ((c1[i] - 'a') - key2[i] + 26) % 26;
        printf("%c", 'a' + dec_val);
    }
    printf(" (Matches \"cashnotneeded\")\n");
    printf("============================================================\n");
    return 0;
}
