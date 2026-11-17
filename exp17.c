#include <stdio.h>
#include <stdint.h>

// Shift schedule for DES encryption (left shifts)
const int ENC_SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Shift schedule for DES decryption (right shifts)
// To get K16 first: 0 shifts
// To get K15: right shift by 1
// Then right shift according to reverse schedule
const int DEC_SHIFTS[16] = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int main() {
    printf("============================================================\n");
    printf(" Experiment 17: DES Decryption Subkey Shift Schedule Design \n");
    printf("============================================================\n\n");

    printf("In DES, the same algorithm is used for encryption and decryption.\n");
    printf("For decryption, the 16 round keys (K1, K2, ..., K16) must be applied\n");
    printf("in reverse order: K16, K15, K14, ..., K2, K1.\n\n");

    printf("Comparison of Encryption vs Decryption Key Generation:\n");
    printf("----------------------------------------------------------------------\n");
    printf(" Round | Enc Key | Left Shift | Dec Round | Dec Key | Right Shift\n");
    printf("-------+---------+------------+-----------+---------+-----------------\n");

    int enc_cum = 0;
    for (int r = 0; r < 16; r++) {
        enc_cum += ENC_SHIFTS[r];
        printf("   %2d  |   K%-2d   |     %d      |    %2d     |   K%-2d   |      %d\n",
               r + 1, r + 1, ENC_SHIFTS[r],
               r + 1, 16 - r, DEC_SHIFTS[r]);
    }
    printf("----------------------------------------------------------------------\n\n");

    printf("Key Generation Analysis:\n");
    printf("1. Total left shifts during encryption = 1+1+2+2+2+2+2+2+1+2+2+2+2+2+2+1 = 28 shifts.\n");
    printf("   Since each half (C and D) is 28 bits, 28 shifts completes a full 360-degree cycle,\n");
    printf("   returning C16 = C0 and D16 = D0!\n\n");
    printf("2. For Decryption:\n");
    printf("   - At round 1 of decryption, (C16, D16) is already available: shift = 0 bits -> K16.\n");
    printf("   - To get K15: shift right by 1 bit (undoing round 16's shift of 1).\n");
    printf("   - To get K14: shift right by 2 bits (undoing round 15's shift of 2).\n");
    printf("   - And so on: right shift of 1 for rounds 2, 9, 16; right shift of 2 for others.\n");
    printf("   - Total right shifts during decryption = 0 + 1 + 2*7 + 1 + 2*5 + 1 = 28 shifts.\n");
    printf("   - Result: Exactly yields K16 down to K1 in pristine order!\n");
    printf("============================================================\n");
    return 0;
}
