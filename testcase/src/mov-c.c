#include "trap.h"

int A[10];
int b;

int main() {
	A[0] = 0;
	printf("0\n");
	A[1] = 1;
	printf("1\n");
	A[2] = 2;
	printf("2\n");
	A[3] = 3;
	printf("3\n");
	A[4] = 4;
	printf("4\n");
	
	b = A[3];
	printf("5\n");
	A[5] = b;
	printf("6\n");

	nemu_assert(A[0] == 0);
	printf("7\n");
	nemu_assert(A[1] == 1);
	printf("8\n");
	nemu_assert(A[2] == 2);
	printf("9\n");
	nemu_assert(A[3] == 3);
	printf("10\n");
	nemu_assert(A[4] == 4);
	printf("11\n");
	nemu_assert(b == 3);
	printf("12\n");
	nemu_assert(A[5] == 3);
	printf("13\n");

	HIT_GOOD_TRAP;

	return 0;
}
