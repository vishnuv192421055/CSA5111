#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char *ENGLISH_FREQ_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} LetterFreq;

int cmpFreq(const void *a, const void *b) {
    return ((LetterFreq *)b)->count - ((LetterFreq *)a)->count;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 37: Automated Monoalphabetic Frequency Attack   \n");
    printf("============================================================\n\n");

    const char *cipher = "WKH TXLFN EURZQ IRA MXPSV RYHU WKH ODCB GRJ";
    int topN = 10;

    printf("Input Ciphertext:\n  \"%s\"\n\n", cipher);

    LetterFreq freqs[26];
    for (int i = 0; i < 26; i++) {
        freqs[i].letter = 'A' + i;
        freqs[i].count = 0;
    }
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) freqs[toupper(cipher[i]) - 'A'].count++;
    }

    qsort(freqs, 26, sizeof(LetterFreq), cmpFreq);

    char baseMap[26];
    for (int i = 0; i < 26; i++) {
        baseMap[freqs[i].letter - 'A'] = ENGLISH_FREQ_ORDER[i];
    }

    printf("Generating Top %d Plaintext Hypotheses without Human Intervention:\n", topN);
    printf("----------------------------------------------------------------------\n");

    int swaps[10][2] = {
        {-1, -1}, // baseline
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {0, 2}, {1, 3}, {4, 5}, {5, 6}, {2, 4}
    };

    for (int k = 0; k < topN; k++) {
        char map[26];
        memcpy(map, baseMap, 26);
        if (swaps[k][0] != -1) {
            int i1 = freqs[swaps[k][0]].letter - 'A';
            int i2 = freqs[swaps[k][1]].letter - 'A';
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
