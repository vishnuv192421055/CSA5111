#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

long long modExp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

long long extendedGCD(long long a, long long b, long long *x, long long *y) {
    if (a == 0) { *x = 0; *y = 1; return b; }
    long long x1, y1;
    long long g = extendedGCD(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return g;
}

long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = extendedGCD(a, m, &x, &y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

int main() {
    printf("============================================================\n");
    printf(" Experiment 32: DSA (Randomized) vs RSA (Deterministic)     \n");
    printf("============================================================\n\n");

    // 1. RSA Demonstration (Deterministic)
    long long rsa_n = 3599, rsa_d = 3031, rsa_e = 31;
    long long msg_hash = 1234;

    long long rsa_sig1 = modExp(msg_hash, rsa_d, rsa_n);
    long long rsa_sig2 = modExp(msg_hash, rsa_d, rsa_n);

    printf("1. RSA DIGITAL SIGNATURE (DETERMINISTIC):\n");
    printf("------------------------------------------------------------\n");
    printf("  Message Digest H(M) = %lld\n", msg_hash);
    printf("  Signature 1 (Occasion 1): S1 = M^d mod n = %lld\n", rsa_sig1);
    printf("  Signature 2 (Occasion 2): S2 = M^d mod n = %lld\n", rsa_sig2);
    printf("  Signatures identical? %s (Always identical without randomized padding)\n\n",
           (rsa_sig1 == rsa_sig2) ? "YES" : "NO");

    // 2. DSA Demonstration (Randomized / Nonce k)
    // Small domain DSA parameters:
    long long p = 283, q = 47; // p is prime, q is prime divisor of p-1 (282 = 6 * 47)
    long long h = 3;
    long long g = modExp(h, (p - 1) / q, p); // g = 3^6 mod 283 = 164
    long long x = 24; // Private signing key (0 < x < q)
    long long y = modExp(g, x, p); // Public key

    printf("2. DSA DIGITAL SIGNATURE (NON-DETERMINISTIC):\n");
    printf("------------------------------------------------------------\n");
    printf("  Domain Parameters: p = %lld, q = %lld, g = %lld\n", p, q, g);
    printf("  Private key x = %lld, Public key y = %lld\n", x, y);
    printf("  Signing SAME message H(M) = 15 on two different occasions:\n\n", msg_hash);

    long long H_M = 15;

    // Occasion 1: Choose k1 = 13
    long long k1 = 13;
    long long r1 = modExp(g, k1, p) % q;
    long long s1 = (modInverse(k1, q) * (H_M + x * r1)) % q;

    // Occasion 2: Choose k2 = 29
    long long k2 = 29;
    long long r2 = modExp(g, k2, p) % q;
    long long s2 = (modInverse(k2, q) * (H_M + x * r2)) % q;

    printf("  Occasion 1 (using ephemeral k = %lld):\n", k1);
    printf("    Signature (r1, s1) = (%lld, %lld)\n", r1, s1);
    printf("  Occasion 2 (using ephemeral k = %lld):\n", k2);
    printf("    Signature (r2, s2) = (%lld, %lld)\n\n", r2, s2);
    printf("  Signatures differ? %s (Completely distinct numbers!)\n\n",
           (r1 != r2 || s1 != s2) ? "YES" : "NO");

    // 3. Critical Implication: Nonce Reuse Vulnerability in DSA
    printf("3. CATASTROPHIC IMPLICATION OF 'k' IN DSA (The Sony PS3 Flaw):\n");
    printf("------------------------------------------------------------\n");
    printf("  If an implementation ever reuses 'k' to sign two different messages M1 and M2:\n");
    long long m1 = 15, m2 = 22;
    long long reused_k = 17;
    long long r_reused = modExp(g, reused_k, p) % q;
    long long s_m1 = (modInverse(reused_k, q) * (m1 + x * r_reused)) % q;
    long long s_m2 = (modInverse(reused_k, q) * (m2 + x * r_reused)) % q;

    printf("  Message 1 (H=%lld) signed with k=%lld -> s1 = %lld\n", m1, reused_k, s_m1);
    printf("  Message 2 (H=%lld) signed with k=%lld -> s2 = %lld (same r = %lld)\n", m2, reused_k, s_m2, r_reused);

    // Attacker derives k and x:
    // s1 - s2 = k^(-1) * (m1 - m2) mod q
    // k = (m1 - m2) / (s1 - s2) mod q
    long long num = (m1 - m2 + q) % q;
    long long den = (s_m1 - s_m2 + q) % q;
    long long recovered_k = (num * modInverse(den, q)) % q;
    // x = (s1 * k - m1) * r^(-1) mod q
    long long recovered_x = (((s_m1 * recovered_k - m1 + 1000 * q) % q) * modInverse(r_reused, q)) % q;

    printf("\n  Adversary computes:\n");
    printf("    Recovered ephemeral k = %lld (Actual k was %lld)\n", recovered_k, reused_k);
    printf("    Recovered Private Key x = %lld (Actual x was %lld)\n", recovered_x, x);
    printf("    Private Key Compromised: %s!\n", (recovered_x == x) ? "YES" : "NO");
    printf("============================================================\n");
    return 0;
}
