#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define ROTL64(x, y) (((x) << (y)) | ((x) >> (64 - (y))))

int main() {
    printf("============================================================\n");
    printf(" Experiment 29: SHA-3 / Keccak State Lane Diffusion Analysis\n");
    printf("============================================================\n\n");

    printf("SHA-3 (Keccak-f[1600]) Parameters:\n");
    printf("  Total state size     : 1600 bits = 25 lanes (each lane 64 bits)\n");
    printf("  Block size (Rate r)  : 1024 bits = 16 lanes (lanes 0..15)\n");
    printf("  Capacity c           :  576 bits =  9 lanes (lanes 16..24)\n\n");

    // 5x5 state matrix
    // Initially, rate lanes (16 lanes) have nonzero bits; capacity lanes (9 lanes) are 0
    uint64_t state[5][5];
    int lane_idx = 0;
    int initial_zeros = 0;

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if (lane_idx < 16) {
                // Rate portion: has nonzero bits
                state[x][y] = 0xAAAAAAAAAAAAAAAAULL ^ (lane_idx + 1);
            } else {
                // Capacity portion: initialized to all zeros
                state[x][y] = 0x0000000000000000ULL;
                initial_zeros++;
            }
            lane_idx++;
        }
    }

    printf("Initial State Matrix (0: all-zero lane, 1: non-zero lane):\n");
    for (int y = 0; y < 5; y++) {
        printf("  Row %d: ", y);
        for (int x = 0; x < 5; x++) {
            printf("[%s] ", (state[x][y] != 0) ? "NON-ZERO" : "  ZERO  ");
        }
        printf("\n");
    }
    printf("Total zero lanes in capacity portion = %d\n\n", initial_zeros);

    printf("Simulating Keccak Round 1 (Theta Step):\n");
    printf("------------------------------------------------------------\n");
    printf("Theta step computes column parity C[x] and parity diffusion D[x]:\n");
    printf("  C[x] = A[x,0] ^ A[x,1] ^ A[x,2] ^ A[x,3] ^ A[x,4]\n");
    printf("  D[x] = C[(x+4)%%5] ^ ROTL(C[(x+1)%%5], 1)\n");
    printf("  A'[x,y] = A[x,y] ^ D[x]\n\n");

    uint64_t C[5];
    for (int x = 0; x < 5; x++) {
        C[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
    }

    uint64_t D[5];
    for (int x = 0; x < 5; x++) {
        D[x] = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1);
    }

    // Apply Theta
    int remaining_zeros = 0;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            state[x][y] ^= D[x];
            if (state[x][y] == 0) remaining_zeros++;
        }
    }

    printf("State Matrix AFTER Round 1 (Theta Step):\n");
    for (int y = 0; y < 5; y++) {
        printf("  Row %d: ", y);
        for (int x = 0; x < 5; x++) {
            printf("[%s] ", (state[x][y] != 0) ? "NON-ZERO" : "  ZERO  ");
        }
        printf("\n");
    }
    printf("\nRemaining zero lanes = %d\n\n", remaining_zeros);

    printf("Conclusion:\n");
    printf("  Because each column 'x' contains at least one rate lane with non-zero bits,\n");
    printf("  every D[x] is non-zero. When D[x] is XORed into the capacity lanes (which were 0),\n");
    printf("  0 ^ D[x] = D[x] != 0.\n");
    printf("  Therefore, it takes exactly 1 ROUND (specifically, the first Theta step of Round 1)\n");
    printf("  before ALL 25 lanes in the internal state matrix contain non-zero bits!\n");
    printf("============================================================\n");
    return 0;
}
