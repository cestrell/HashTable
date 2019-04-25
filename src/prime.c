#include <math.h>
#include "prime.h"

/*
	Check if a number is prime

	1  - Prime
	0  - Not Prime
	-1 - Undefined
*/
int is_prime(const int x) {
	if (x < 2) return -1;
	if (x < 4) return 1;
	if ((x % 2) == 0) return 0;

	int cmp = floor(sqrt((double)x));
	for (int factor = 3; factor <= cmp; factor += 2) {
		if ((x % i) == 0) return 0;
	}
	return 1;
}

int next_prime(int x) {
	while (is_prime(x) != 1) x++;
	return x;
}
