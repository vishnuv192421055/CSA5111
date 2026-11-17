#include <stdio.h>
#include <math.h>

int main() {
    printf("============================================================\n");
    printf(" Experiment 11: Keyspace Analysis of Playfair Cipher        \n");
    printf("============================================================\n\n");

    // Total letters placed in 5x5 matrix = 25 (I and J combined)
    // Total permutations = 25!
    double fact25 = 1.0;
    for (int i = 1; i <= 25; i++) fact25 *= i;

    double log2_25 = log2(fact25);

    printf("1. Total Possible Keys (ignoring equivalent matrices):\n");
    printf("   A 5x5 matrix contains 25 distinct symbols (combining I and J).\n");
    printf("   Total permutations = 25!\n");
    printf("   25! = %.6e\n", fact25);
    printf("   In powers of 2: 2^(%.2f) ~= 2^84 keys.\n\n", log2_25);

    // Part a: Accounting for identical encryption results
    // Shifting rows cyclically (5 ways) and columns cyclically (5 ways)
    // yields identical relative positions for all digraph rules: 5 * 5 = 25 shifts.
    double fact24 = fact25 / 25.0; // equal to 24!
    double log2_24 = log2(fact24);

    printf("(a) Accounting for Equivalent Key Symmetries:\n");
    printf("    In Playfair cipher, any cyclic shift of the rows (5 possibilities)\n");
    printf("    and any cyclic shift of the columns (5 possibilities) leaves the relative\n");
    printf("    horizontal and vertical relationships unchanged. Thus, each key belongs\n");
    printf("    to an equivalence class of 5 x 5 = 25 identical-encryption keys.\n\n");
    printf("    Effectively Unique Keys = 25! / 25 = 24!\n");
    printf("    24! = %.6e\n", fact24);
    printf("    In powers of 2: 2^(%.2f) ~= 2^79 effectively unique keys.\n", log2_24);
    printf("============================================================\n");
    return 0;
}
