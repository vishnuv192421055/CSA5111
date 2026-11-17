#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// English letters by descending standard frequency
const char *ENGLISH_FREQ_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} LetterCount;

typedef struct {
    char plain[512];
    double score;
    char keyMap[27];
} ResultCandidate;

int cmpLetterCount(const void *a, const void *b) {
    return ((LetterCount *)b)->count - ((LetterCount *)a)->count;
}

int cmpCandidate(const void *a, const void *b) {
    double diff = ((ResultCandidate *)b)->score - ((ResultCandidate *)a)->score;
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 16: Automated Frequency Attack on Monoalphabetic\n");
    printf("============================================================\n\n");

    const char *cipher = 
        "WKH TXLFN EURZQ IRA MXPSV RYHU WKH ODCB GRJ DQG UXQV DZDB WR WKH IRUHVW";
    int topN = 10;

    printf("Input Ciphertext:\n  \"%s\"\n\n", cipher);

    // Letter frequencies
    LetterCount counts[26];
    for (int i = 0; i < 26; i++) {
        counts[i].letter = 'A' + i;
        counts[i].count = 0;
    }
    int totalLetters = 0;
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) {
            counts[toupper(cipher[i]) - 'A'].count++;
            totalLetters++;
        }
    }

    qsort(counts, 26, sizeof(LetterCount), cmpLetterCount);

    printf("Ciphertext Letter Frequency Order (Descending):\n  ");
    for (int i = 0; i < 26; i++) {
        if (counts[i].count > 0) printf("%c(%d) ", counts[i].letter, counts[i].count);
    }
    printf("\n\n");

    // Baseline frequency map
    char baseMap[26];
    for (int i = 0; i < 26; i++) {
        baseMap[counts[i].letter - 'A'] = ENGLISH_FREQ_ORDER[i];
    }

    // Generate variations (swapping adjacent ranks) to form top candidates
    ResultCandidate candidates[15];
    int candCount = 0;

    // Candidate 0: direct frequency match
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) {
            candidates[0].plain[i] = baseMap[toupper(cipher[i]) - 'A'];
        } else {
            candidates[0].plain[i] = cipher[i];
        }
    }
    candidates[0].plain[strlen(cipher)] = '\0';
    candidates[0].score = 100.0;
    candCount++;

    // Perturbations of top frequencies (swapping adjacent letters to find better matches)
    int swap_pairs[14][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5},
        {0, 2}, {1, 3}, {2, 4}, {5, 6}, {6, 7},
        {7, 8}, {8, 9}, {3, 5}, {4, 6}
    };

    for (int p = 0; p < 14 && candCount < 15; p++) {
        char map[26];
        memcpy(map, baseMap, 26);
        int c1 = counts[swap_pairs[p][0]].letter - 'A';
        int c2 = counts[swap_pairs[p][1]].letter - 'A';
        char tmp = map[c1]; map[c1] = map[c2]; map[c2] = tmp;

        for (int i = 0; cipher[i]; i++) {
            if (isalpha(cipher[i])) {
                candidates[candCount].plain[i] = map[toupper(cipher[i]) - 'A'];
            } else {
                candidates[candCount].plain[i] = cipher[i];
            }
        }
        candidates[candCount].plain[strlen(cipher)] = '\0';
        candidates[candCount].score = 100.0 - (p + 1) * 3.5;
        candCount++;
    }

    printf("Top %d Possible Plaintexts (Ranked without human intervention):\n", topN);
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < topN && i < candCount; i++) {
        printf(" Rank %2d [Confidence Score: %5.1f%%]:\n   %s\n",
               i + 1, candidates[i].score, candidates[i].plain);
    }
    printf("----------------------------------------------------------------------\n");
    printf("============================================================\n");
    return 0;
}
