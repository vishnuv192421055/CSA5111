#include <stdio.h>
#include <stdint.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b; b = a % b; a = t;
    }
    return a;
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
    printf(" Experiment 28: Analysis of Modified Diffie-Hellman Protocol\n");
    printf(" Proposal: Send x^a mod q instead of a^x mod q              \n");
    printf("============================================================\n\n");

    long long q = 353; // Public prime modulus
    long long a = 7;   // Public power with gcd(a, q-1) = 1

    printf("Public Parameters:\n");
    printf("  Prime modulus q = %lld (Euler totient phi(q) = q - 1 = %lld)\n", q, q - 1);
    printf("  Public power  a = %lld\n", a);
    printf("  gcd(a, q - 1)   = %lld\n\n", gcd(a, q - 1));

    // Alice and Bob choose secret numbers
    long long x_A = 97;  // Alice's secret
    long long x_B = 233; // Bob's secret

    printf("Participants' Secrets:\n");
    printf("  Alice's secret x_A = %lld\n", x_A);
    printf("  Bob's secret   x_B = %lld\n\n", x_B);

    // Exchanged public values: Y = x^a mod q
    long long Y_A = modExp(x_A, a, q);
    long long Y_B = modExp(x_B, a, q);

    printf("Values Transmitted Over Public Channel:\n");
    printf("  Alice sends Y_A = (x_A)^a mod q = %lld^%lld mod %lld = %lld\n", x_A, a, q, Y_A);
    printf("  Bob sends   Y_B = (x_B)^a mod q = %lld^%lld mod %lld = %lld\n\n", x_B, a, q, Y_B);

    printf("Analysis of Questions:\n");
    printf("------------------------------------------------------------\n");

    printf("1. What would happen if participants sent x^a mod q instead of a^x mod q?\n");
    printf("   -> The mathematical foundation changes from the hard Discrete Logarithm\n");
    printf("      Problem (finding x from a^x mod q) to the Modular Root Problem.\n");
    printf("      Since q is prime, phi(q) = q - 1 is publicly known to everyone!\n\n");

    printf("2. Can Eve find the secret numbers?\n");
    long long inv_a = modInverse(a, q - 1);
    printf("   -> YES! Eve can easily compute the modular inverse of 'a' modulo (q - 1):\n");
    printf("      b = a^(-1) mod (q - 1) = %lld^(-1) mod %lld = %lld\n", a, q - 1, inv_a);
    printf("      Verification: (a * b) mod (q - 1) = (%lld * %lld) mod %lld = %lld\n\n",
           a, inv_a, q - 1, (a * inv_a) % (q - 1));

    long long eve_xA = modExp(Y_A, inv_a, q);
    long long eve_xB = modExp(Y_B, inv_a, q);
    printf("   Eve recovers Alice's secret: (Y_A)^b mod q = %lld^%lld mod %lld = %lld\n",
           Y_A, inv_a, q, eve_xA);
    printf("   Eve recovers Bob's secret  : (Y_B)^b mod q = %lld^%lld mod %lld = %lld\n",
           Y_B, inv_a, q, eve_xB);
    printf("   Match Alice's actual secret: %s\n", (eve_xA == x_A) ? "YES" : "NO");
    printf("   Match Bob's actual secret  : %s\n\n", (eve_xB == x_B) ? "YES" : "NO");

    printf("3. Can Eve break the system without finding the secret numbers?\n");
    printf("   -> YES! Any key that Alice and Bob could compute from public messages Y_A, Y_B\n");
    printf("      can be computed identically by Eve because Eve knows everything they exchange.\n\n");

    printf("Conclusion:\n");
    printf("  Standard Diffie-Hellman relies on a^x mod q being a one-way function.\n");
    printf("  Inverting x^a mod q is trivial when the modulus factorization/totient is public.\n");
    printf("============================================================\n");
    return 0;
}
