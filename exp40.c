#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char *ENG_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} LetterF;

int cmpLF(const void *a, const void *b) {
    return ((LetterF *)b)->count - ((LetterF *)a)->count;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 40: Monoalphabetic Substitution Frequency Attack\n");
    printf("============================================================\n\n");

    const char *cipher = "GSRH RH Z HVXIVG NVHHZTV HLOEVW YB HEYHGRGFGRLM";
    int topN = 10;

    printf("Input Ciphertext:\n  \"%s\"\n\n", cipher);

    LetterF lf[26];
    for (int i = 0; i < 26; i++) {
        lf[i].letter = 'A' + i;
        lf[i].count = 0;
    }
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) lf[toupper(cipher[i]) - 'A'].count++;
    }

    qsort(lf, 26, sizeof(LetterF), cmpLF);

    char baseMap[26];
    for (int i = 0; i < 26; i++) {
        baseMap[lf[i].letter - 'A'] = ENG_ORDER[i];
    }

    printf("Top %d Possible Plaintexts (Automated Frequency Ranking):\n", topN);
    printf("----------------------------------------------------------------------\n");

    int swaps[10][2] = {
        {-1, -1}, {0, 1}, {1, 2}, {2, 3}, {3, 4},
        {0, 2}, {1, 3}, {4, 5}, {2, 4}, {5, 6}
    };

    for (int k = 0; k < topN; k++) {
        char map[26];
        memcpy(map, baseMap, 26);
        if (swaps[k][0] != -1) {
            int i1 = lf[swaps[k][0]].letter - 'A';
            int i2 = lf[swaps[k][1]].letter - 'A';
            char t = map[i1]; map[i1] = map[i2]; map[i2] = t;
        }

        char out[128];
        int len = 0;
        for (int i = 0; cipher[i]; i++) {
            if (isalpha(cipher[i])) {
                out[len++] = map[toupper(cipher[i]) - 'A'];
            } else {
                out[len++] = cipher[i];
            }
        }
        out[len] = '\0';
        printf(" Rank %2d: %s\n", k + 1, out);
    }
    printf("----------------------------------------------------------------------\n");
    printf("============================================================\n");
    return 0;
}
