#ifndef _MATHS_H_
#define _MATHS_H_

#include <vector>

long floor_sqrt(long);

bool is_prime_v1(long);
bool is_prime_v2(long);

inline bool is_prime(long number) { return is_prime_v2(number); }

long prime_ge(long);

std::vector<long> factors(long);
std::vector<long> prime_factors(long);

long gcd_v1(long, long);
long gcd_v2(long, long);
long gcd_v3(long, long);
long gcd_v4(long, long);
long gcd_v5(long, long);

inline long gcd(long m, long n) { return gcd_v5(m, n); }

long lcm_v1(long, long);
long lcm_v2(long, long);
long lcm_v3(long, long);

inline long lcm(long m, long n) { return lcm_v3(m, n); }

#endif // _MATHS_H_
