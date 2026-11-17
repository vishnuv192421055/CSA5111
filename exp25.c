#include <stdio.h>
#include <stdint.h>

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
        *x = 0;
        *y = 1;
        return b;
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
    printf(" Experiment 25: RSA Cryptanalysis via Shared Factor with n  \n");
    printf("============================================================\n\n");

    // Public Key setup
    long long p_orig = 61, q_orig = 53;
    long long n = p_orig * q_orig; // n = 3233
    long long e = 17;

    printf("Public Parameters Known to Attacker:\n");
    printf("  Modulus n = %lld\n", n);
    printf("  Public Exponent e = %lld\n\n", e);

    // Suppose an intercepted plaintext block M has a factor in common with n
    // For instance, M is a multiple of p_orig: M = 3 * 61 = 183
    long long known_M = 183;
    printf("Information Leaked to Attacker:\n");
    printf("  \"One of the plaintext blocks M = %lld shares a common factor with n.\"\n\n", known_M);

    printf("Question: Does this help us in any way?\n");
    printf("------------------------------------------------------------\n");
    printf("Answer: YES, IT COMPLETELY BREAKS THE RSA CRYPTOSYSTEM!\n\n");

    printf("Execution of the Attack:\n");
    // Step 1: Calculate gcd(known_M, n)
    long long factor = gcd(known_M, n);
    printf("1. Compute factor = gcd(M, n) = gcd(%lld, %lld):\n", known_M, n);
    printf("   gcd(%lld, %lld) = %lld\n\n", known_M, n, factor);

    if (factor > 1 && factor < n) {
        long long p = factor;
        long long q = n / p;
        printf("2. Factorization of n obtained instantaneously:\n");
        printf("   Prime p = %lld\n", p);
        printf("   Prime q = n / p = %lld\n\n", q);

        long long phi = (p - 1) * (q - 1);
        printf("3. Calculate Euler's Totient phi(n):\n");
        printf("   phi(n) = (%lld - 1) * (%lld - 1) = %lld\n\n", p, q, phi);

        long long d = modInverse(e, phi);
        printf("4. Derive the Secret Private Key d:\n");
        printf("   d = e^(-1) mod phi(n) = %lld^(-1) mod %lld = %lld\n\n", e, phi, d);

        // Demonstrate decrypting an unknown secret message
        long long secret_message = 2026;
        long long secret_cipher = modExp(secret_message, e, n);
        long long cracked_message = modExp(secret_cipher, d, n);

        printf("5. Decrypting ANY other intercepted secret ciphertext:\n");
        printf("   Intercepted Ciphertext C = %lld\n", secret_cipher);
        printf("   Decrypted using cracked d: M' = C^d mod n = %lld\n", cracked_message);
        printf("   Matches Original Message: %s\n", (cracked_message == secret_message) ? "YES (Fully Broken!)" : "NO");
    }
    printf("============================================================\n");
    return 0;
}
