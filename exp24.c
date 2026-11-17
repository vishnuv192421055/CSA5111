#include <stdio.h>
#include <math.h>

// Extended Euclidean Algorithm
// Finds x and y such that a*x + b*y = gcd(a, b)
int extendedGCD(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = extendedGCD(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

int modInverse(int e, int phi) {
    int x, y;
    int g = extendedGCD(e, phi, &x, &y);
    if (g != 1) return -1;
    int res = (x % phi + phi) % phi;
    return res;
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
    printf(" Experiment 24: RSA Private Key Calculation (e=31, n=3599)  \n");
    printf("============================================================\n\n");

    int e = 31;
    int n = 3599;

    printf("Given Public Key Parameters:\n");
    printf("  Public exponent e = %d\n", e);
    printf("  Modulus         n = %d\n\n", n);

    // Step 1: Trial-and-error factorization of n
    printf("Step 1: Factoring n = 3599 into primes p and q:\n");
    int p = 0, q = 0;
    int limit = (int)sqrt(n) + 1;
    for (int i = 2; i <= limit; i++) {
        if (n % i == 0) {
            p = i;
            q = n / i;
            break;
        }
    }

    printf("  Searched divisors up to sqrt(3599) ~= %d\n", limit);
    printf("  Found factors: p = %d, q = %d\n", p, q);
    printf("  Verification : %d * %d = %d\n\n", p, q, p * q);

    // Step 2: Compute Euler's Totient function phi(n)
    int phi = (p - 1) * (q - 1);
    printf("Step 2: Computing Euler's Totient phi(n):\n");
    printf("  phi(n) = (p - 1) * (q - 1) = (%d - 1) * (%d - 1) = %d * %d = %d\n\n",
           p, q, p - 1, q - 1, phi);

    // Step 3: Extended Euclidean Algorithm to find d = e^(-1) mod phi(n)
    printf("Step 3: Finding Private Key 'd' using Extended Euclidean Algorithm:\n");
    printf("  Solve: %d * d = 1 (mod %d)\n", e, phi);

    int d = modInverse(e, phi);
    printf("  Multiplicative inverse d = %d\n\n", d);

    printf("Verification:\n");
    printf("  (e * d) mod phi(n) = (%d * %d) mod %d = %lld mod %d = %lld\n",
           e, d, phi, (long long)e * d, phi, ((long long)e * d) % phi);
    printf("  Private Key of User: d = %d\n\n", d);

    // Test encryption and decryption with sample message
    long long M = 1234; // Plaintext message
    long long C = modExp(M, e, n);
    long long dec_M = modExp(C, d, n);

    printf("Demonstration of RSA Encryption & Decryption:\n");
    printf("  Plaintext M = %lld\n", M);
    printf("  Ciphertext C = M^e mod n = %lld^%d mod %d = %lld\n", M, e, n, C);
    printf("  Decrypted M' = C^d mod n = %lld^%d mod %d = %lld\n", C, d, n, dec_M);
    printf("  Status: %s\n", (M == dec_M) ? "SUCCESS (Matched!)" : "FAILED");
    printf("============================================================\n");
    return 0;
}
