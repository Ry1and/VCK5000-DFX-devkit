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

#define printhex(chars) for (int i = 0; i < sizeof(chars); i++) printf("%02hhx ", chars[i])
#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define int_from_bytes(bytes) int((unsigned char)bytes[0] | (unsigned char)bytes[1] << 8 | (unsigned char)bytes[2] << 16 | (unsigned char)bytes[3] << 24)

char *gen_bytes(char *dest, long long size);
char *bytes_from_int(char *dest, int val);

/*
	Generate N random byte
*/
char *gen_bytes(char *dest, long long size) {
    int rand_int = rand();
    memcpy(dest, &rand_int, 4);
    printf("%x\n", int_from_bytes(dest));
	return dest;
}

/*
	Generate bytes from an integer value
*/
char *bytes_from_int(char *dest, int val){
	memcpy(dest, &val, 4);
	return dest;
}




#endif




