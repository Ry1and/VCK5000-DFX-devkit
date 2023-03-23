#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>

#include "utils.h"



/*
	Generate N random byte
*/
char *gen_bytes(int size) {
	srand(time(0));
	char *dest = (char *)malloc(size);
	int nwords = size / 4;
	printf("%d\n", nwords);
	int *buffer = (int *)malloc((nwords + 1) * 4);
	for (int i = 0; i <= nwords; i++ ){
		buffer[i] = rand();
		//printf("%08x", buffer[i]);
	}

    memcpy(dest, buffer, size);
	return dest;
}

/*
	Generate bytes from an integer value
*/
char *bytes_from_int(int val){
	char *dest = (char *)malloc(4);
	memcpy(dest, &val, 4);
	return dest;
}
