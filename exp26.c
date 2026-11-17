#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long extendedGCD(long long a, long long b, long long *x, long long *y) {
    if (a == 0) {
        *x = 0; *y = 1; return b;
    }
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

int main() {
    printf("============================================================\n");
    printf(" Experiment 26: Security Flaw in Reusing RSA Modulus 'n'    \n");
    printf("============================================================\n\n");

    // Bob's initial setup
    long long p = 101, q = 113;
    long long n = p * q; // 11413
    long long phi = (p - 1) * (q - 1); // 100 * 112 = 11200
    long long e_old = 3533;
    long long d_old = modInverse(e_old, phi); // 6597

    printf("Bob's Setup:\n");
    printf("  Modulus n   = %lld\n", n);
    printf("  Public Key  : e = %lld\n", e_old);
    printf("  Private Key : d = %lld (LEAKED TO EVE!)\n\n", d_old);

    printf("Bob's Flawed Strategy:\n");
    printf("  Bob generates a new keypair (e_new = 17, d_new) but REUSES n.\n");
    printf("  Bob believes his new private key is secure.\n\n");

    printf("Is this safe? -> ABSOLUTELY NOT!\n");
    printf("------------------------------------------------------------\n");
    printf("Attack Proof (Factoring n from known e and d):\n");
    printf("1. Notice that e * d - 1 is a multiple of phi(n) = (p-1)(q-1).\n");
    long long k_phi = e_old * d_old - 1;
    printf("   k_phi = e * d - 1 = %lld * %lld - 1 = %lld\n", e_old, d_old, k_phi);

    // Factor out powers of 2: k_phi = 2^s * t
    long long t = k_phi;
    int s = 0;
    while (t % 2 == 0) {
        t /= 2;
        s++;
    }
    printf("   Written as 2^s * t: s = %d, t = %lld (odd)\n\n", s, t);

    // Find non-trivial square root of 1 mod n
    long long found_p = 0, found_q = 0;
    for (long long a = 2; a < 100; a++) {
        if (gcd(a, n) > 1) {
            found_p = gcd(a, n);
            found_q = n / found_p;
            break;
        }
        long long v = modExp(a, t, n);
        if (v == 1 || v == n - 1) continue;

        for (int i = 1; i <= s; i++) {
            long long v_next = (v * v) % n;
            if (v_next == 1) {
                // v is a non-trivial square root of 1 mod n!
                found_p = gcd(v - 1, n);
                found_q = n / found_p;
                break;
            }
            v = v_next;
        }
        if (found_p > 1 && found_p < n) break;
    }

    printf("2. Factorization recovered by Eve:\n");
    printf("   Prime factor p = %lld\n", found_p);
    printf("   Prime factor q = %lld\n", found_q);
    printf("   Verification: %lld * %lld = %lld\n\n", found_p, found_q, found_p * found_q);

    // Eve now computes phi(n)
    long long eve_phi = (found_p - 1) * (found_q - 1);

    // Bob publishes new e_new = 17
    long long e_new = 17;
    long long bob_d_new = modInverse(e_new, phi);
    long long eve_cracked_d = modInverse(e_new, eve_phi);

    printf("3. Bob announces new public exponent e_new = %lld\n", e_new);
    printf("   Bob's new secret d_new : %lld\n", bob_d_new);
    printf("   Eve's derived d_new    : %lld\n", eve_cracked_d);
    printf("   Does Eve have Bob's new key? %s\n\n", (bob_d_new == eve_cracked_d) ? "YES (INSTANTLY CRACKED!)" : "NO");

    printf("Conclusion:\n");
    printf("  Once a private key d is compromised, the modulus n is PERMANENTLY broken.\n");
    printf("  Bob MUST generate fresh prime factors p, q and a NEW modulus n.\n");
    printf("============================================================\n");
    return 0;
}
