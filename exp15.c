#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Standard English letter frequencies (in percentage)
const double ENGLISH_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150,
    1.974, 0.074
};

typedef struct {
    int shift;
    double score;
    char plaintext[256];
} Candidate;

int compareCandidates(const void *a, const void *b) {
    Candidate *c1 = (Candidate *)a;
    Candidate *c2 = (Candidate *)b;
    if (c2->score > c1->score) return 1;
    if (c2->score < c1->score) return -1;
    return 0;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 15: Automated Frequency Attack on Additive Cipher\n");
    printf("============================================================\n\n");

    const char *sample_cipher = "WKLV LV D VHFUHW PHVVDJH HQFUBSWHG ZLWK FDHVDU FLSKHU";
    int topN = 10;

    printf("Input Ciphertext:\n  \"%s\"\n\n", sample_cipher);

    // Count letter frequencies in ciphertext
    int counts[26] = {0};
    int totalLetters = 0;
    for (int i = 0; sample_cipher[i]; i++) {
        if (isalpha(sample_cipher[i])) {
            counts[toupper(sample_cipher[i]) - 'A']++;
            totalLetters++;
        }
    }

    Candidate candidates[26];
    for (int k = 0; k < 26; k++) {
        candidates[k].shift = k;
        double score = 0.0;
        // Compute correlation with English letter frequencies
        for (int i = 0; i < 26; i++) {
            int plainLetter = (i - k + 26) % 26;
            score += ((double)counts[i] / totalLetters) * ENGLISH_FREQ[plainLetter];
        }
        candidates[k].score = score;

        // Generate candidate plaintext
        int len = 0;
        for (int i = 0; sample_cipher[i]; i++) {
            if (isalpha(sample_cipher[i])) {
                char base = isupper(sample_cipher[i]) ? 'A' : 'a';
                candidates[k].plaintext[len++] = (char)(((toupper(sample_cipher[i]) - 'A' - k + 26) % 26) + base);
            } else {
                candidates[k].plaintext[len++] = sample_cipher[i];
            }
        }
        candidates[k].plaintext[len] = '\0';
    }

    // Sort by correlation score descending
    qsort(candidates, 26, sizeof(Candidate), compareCandidates);

    printf("Top %d Most Likely Plaintexts (Ranked by Frequency Correlation Score):\n", topN);
    printf("----------------------------------------------------------------------\n");
    printf(" Rank | Shift (k) |  Score  | Decrypted Plaintext\n");
    printf("------+-----------+---------+-----------------------------------------\n");
    for (int i = 0; i < topN && i < 26; i++) {
        printf("  %2d  |     %2d    | %7.4f | %s\n",
               i + 1, candidates[i].shift, candidates[i].score, candidates[i].plaintext);
    }
    printf("----------------------------------------------------------------------\n");
    printf("Rank 1 shows the automated best match without any human intervention!\n");
    printf("============================================================\n");
    return 0;
}
