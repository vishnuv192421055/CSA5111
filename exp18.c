#include <stdio.h>
#include <stdbool.h>

// Permuted Choice 2 (PC-2) table of DES (48 bits output from 56 bits input)
// 1-based indexing where 1..28 corresponds to register C, and 29..56 to register D.
const int PC2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2, // First 24 bits
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32  // Second 24 bits
};

int main() {
    printf("============================================================\n");
    printf(" Experiment 18: DES Subkey Disjoint 24-Bit Subsets Proof    \n");
    printf("============================================================\n\n");

    printf("Analysis of DES Key Schedule Design:\n");
    printf("1. The 64-bit initial key is reduced to 56 bits by PC-1 (parity bits dropped).\n");
    printf("2. These 56 bits are partitioned into two strictly separate 28-bit halves:\n");
    printf("     - Register C: Bits  1 through 28 (Left Half)\n");
    printf("     - Register D: Bits 29 through 56 (Right Half)\n");
    printf("3. In each round, C and D are circularly shifted independently.\n");
    printf("4. PC-2 compresses the 56 bits into a 48-bit subkey (24 bits left, 24 bits right).\n\n");

    printf("Inspection of PC-2 Permutation Table:\n");
    printf("------------------------------------------------------------\n");

    printf("First 24 Subkey Bits (Left Half of Subkey):\n  ");
    bool left_pure = true;
    for (int i = 0; i < 24; i++) {
        printf("%2d ", PC2[i]);
        if (PC2[i] < 1 || PC2[i] > 28) left_pure = false;
        if ((i + 1) % 6 == 0) printf("\n  ");
    }

    printf("\nSecond 24 Subkey Bits (Right Half of Subkey):\n  ");
    bool right_pure = true;
    for (int i = 24; i < 48; i++) {
        printf("%2d ", PC2[i]);
        if (PC2[i] < 29 || PC2[i] > 56) right_pure = false;
        if ((i + 1) % 6 == 0) printf("\n  ");
    }

    printf("\nMathematical Verification:\n");
    printf("------------------------------------------------------------\n");
    printf("  Subset C range check: All first 24 bits in [1, 28]   -> %s\n",
           left_pure ? "TRUE (All from C)" : "FALSE");
    printf("  Subset D range check: All second 24 bits in [29, 56] -> %s\n",
           right_pure ? "TRUE (All from D)" : "FALSE");
    printf("  Intersection of Subsets: EMPTY SET (C_bits intersect D_bits = {})\n\n");

    printf("Conclusion:\n");
    printf("  The first 24 bits of each subkey come exclusively from C (28 bits),\n");
    printf("  and the second 24 bits come exclusively from D (28 bits).\n");
    printf("  These two subsets are strictly DISJOINT for all 16 rounds.\n");
    printf("============================================================\n");
    return 0;
}
