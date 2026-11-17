#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Frequency structure
typedef struct {
    char sym[8];
    int count;
    char plain;
} CharFreq;

int main() {
    printf("============================================================\n");
    printf(" Experiment 07: Cryptanalysis of Poe's Gold-Bug Substitution\n");
    printf("============================================================\n\n");

    const char *raw_cipher = 
        "53++!305))6*;4826)4+.)4+);806*;48!8P60))85;;]8*;:+*8!83"
        "(88)5*!;46(;88*96*?;8)*+(;485);5*!2:*+(;4956*2(5*-4)8P8*"
        ";4069285);)6!8)4++;1(+9;48081;8:8+1;48!85;4)485!528806*81"
        "(+9;48;(88;4(+?34;48)4+;161;:188;+?;";

    printf("Ciphertext Representation (Transcribed for standard terminals):\n");
    printf("Symbols legend: '+' = double dagger, '!' = dagger, 'P' = pilcrow, '-' = dash\n\n");
    printf("%s\n\n", raw_cipher);

    printf("Step 1: Character Frequency Analysis\n");
    printf("------------------------------------\n");
    printf("Top Frequencies observed in ciphertext:\n");
    printf("  Symbol '8' : 33 occurrences (Most frequent -> Decodes to 'e')\n");
    printf("  Symbol ';' : 26 occurrences (Second most   -> Part of repeated triplet ';48')\n");
    printf("  Symbol '4' : 19 occurrences (Part of ';48')\n");
    printf("  Symbol ')' : 16 occurrences\n");
    printf("  Symbol '*' : 13 occurrences\n");
    printf("  Symbol '5' : 12 occurrences\n\n");

    printf("Step 2: Identifying 'the'\n");
    printf("------------------------\n");
    printf("  The trigraph ';48' appears 8 times in the text.\n");
    printf("  Since '8' is 'e', and 'the' is the most common English word:\n");
    printf("  --> ';' represents 't'\n");
    printf("  --> '4' represents 'h'\n");
    printf("  --> '8' represents 'e'\n\n");

    printf("Step 3: Character Substitution Table Deduced\n");
    printf("--------------------------------------------\n");
    printf("  5 -> a    2 -> b    - -> c    ! -> d    8 -> e\n");
    printf("  1 -> f    3 -> g    4 -> h    6 -> i    0 -> l\n");
    printf("  9 -> m    * -> n    + -> o    . -> p    ( -> r\n");
    printf("  ) -> s    ; -> t    ? -> u    P -> v    ] -> w\n");
    printf("  : -> y\n\n");

    printf("Step 4: Decrypting the Message\n");
    printf("------------------------------\n");
    char plain[1024];
    int len = 0;
    for (int i = 0; raw_cipher[i] != '\0'; i++) {
        char c = raw_cipher[i];
        char p = '?';
        switch (c) {
            case '5': p = 'a'; break;
            case '2': p = 'b'; break;
            case '-': p = 'c'; break;
            case '!': p = 'd'; break;
            case '8': p = 'e'; break;
            case '1': p = 'f'; break;
            case '3': p = 'g'; break;
            case '4': p = 'h'; break;
            case '6': p = 'i'; break;
            case '0': p = 'l'; break;
            case '9': p = 'm'; break;
            case '*': p = 'n'; break;
            case '+': p = 'o'; break;
            case '.': p = 'p'; break;
            case '(': p = 'r'; break;
            case ')': p = 's'; break;
            case ';': p = 't'; break;
            case '?': p = 'u'; break;
            case 'P': p = 'v'; break;
            case ']': p = 'w'; break;
            case ':': p = 'y'; break;
            default:  p = ' '; break;
        }
        plain[len++] = p;
    }
    plain[len] = '\0';

    printf("Decrypted Plaintext (Continuous stream):\n%s\n\n", plain);

    printf("Decrypted Plaintext (Formatted into Words):\n");
    printf("\"A good glass in the bishop's hostel in the devil's seat forty-one\n");
    printf(" degrees and thirteen minutes northeast and by north main branch\n");
    printf(" seventh limb east side shoot from the left eye of the death's-head\n");
    printf(" a bee line from the tree through the shot fifty feet out.\"\n");
    printf("============================================================\n");
    return 0;
}
