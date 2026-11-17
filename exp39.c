#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const double ENGLISH_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150,
    1.974, 0.074
};

typedef struct {
    int shift;
    double score;
    char text[256];
} Cand;

int cmpCand(const void *a, const void *b) {
    if (((Cand *)b)->score > ((Cand *)a)->score) return 1;
    if (((Cand *)b)->score < ((Cand *)a)->score) return -1;
    return 0;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 39: Additive Cipher Letter Frequency Attack     \n");
    printf("============================================================\n\n");

    const char *cipher = "KHOOR ZRUOG WKLV LV DQ DGGLWLYH FLSKHU GHPRQVWUDWLRQ";
    int topN = 10;

    printf("Ciphertext Intercepted:\n  \"%s\"\n\n", cipher);

    int counts[26] = {0}, total = 0;
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) { counts[toupper(cipher[i]) - 'A']++; total++; }
    }

    Cand list[26];
    for (int k = 0; k < 26; k++) {
        list[k].shift = k;
        double s = 0.0;
        for (int i = 0; i < 26; i++) {
            s += ((double)counts[i] / total) * ENGLISH_FREQ[(i - k + 26) % 26];
        }
        list[k].score = s;
        int len = 0;
        for (int i = 0; cipher[i]; i++) {
            if (isalpha(cipher[i])) {
                list[k].text[len++] = ((toupper(cipher[i]) - 'A' - k + 26) % 26) + 'A';
            } else {
                list[k].text[len++] = cipher[i];
            }
        }
        list[k].text[len] = '\0';
    }

    qsort(list, 26, sizeof(Cand), cmpCand);

    printf("Top %d Candidate Plaintexts (Ranked by Likelihood Score):\n", topN);
    printf("----------------------------------------------------------------------\n");
    printf(" Rank | Shift (k) | Likelihood | Decrypted Text\n");
    printf("------+-----------+------------+--------------------------------------\n");
    for (int i = 0; i < topN; i++) {
        printf("  %2d  |     %2d    |   %6.4f   | %s\n",
               i + 1, list[i].shift, list[i].score, list[i].text);
    }
    printf("----------------------------------------------------------------------\n");
    printf("============================================================\n");
    return 0;
}
