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

#endif // _MATHS_H_
