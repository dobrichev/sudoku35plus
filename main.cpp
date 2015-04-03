#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern int doMinusPlusByDepth();

int main(int argc, char* argv[])
{
	clock_t start, finish;
	int ret;
	start = clock();
	
	ret = doMinusPlusByDepth();

	finish = clock();
	fprintf(stderr, "\nTotal time %2.3f seconds.\n", (double)(finish - start) / CLOCKS_PER_SEC);
	return ret;
}
