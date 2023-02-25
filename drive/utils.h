#ifndef __UTILS_H
#define __UTILS_H

#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <vector>

#define printhex(chars) for (int i = 0; i < sizeof(chars); i++) printf("%02hhx ", chars[i]); 

#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())


int int_from_bytes(char *bytes);
char *gen_bytes(char *dest, long long size);
char *bytes_from_int(char *dest, int val);

char *gen_bytes(char *dest, long long size) {
    int rand_int = rand();
    memcpy(dest, &rand_int, 4);
    printf("%x\n", int_from_bytes(dest));
	return dest;
}


char *bytes_from_int(char *dest, int val){
	memcpy(dest, &val, 4);
	return dest;
}


int int_from_bytes(char *bytes) {
	int res = 0;
	for (int i = 0; i < 4; i++) {
		printf("res: %x\n", res);
		printf("bytes: %x\n", bytes[i]);
		res ^= bytes[i] << (i * 8);
	}
	return res;
}


#endif




