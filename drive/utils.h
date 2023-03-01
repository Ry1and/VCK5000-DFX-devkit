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
#include <cstdlib>

#define printhex(chars, size) for (int i = 0; i < size; i++) printf("%02hhx", chars[i])
#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define int_from_bytes(bytes) int((unsigned char)bytes[0] | (unsigned char)bytes[1] << 8 | (unsigned char)bytes[2] << 16 | (unsigned char)bytes[3] << 24)

char *gen_bytes(int size);
char *bytes_from_int(int val);



/*
	Generate N random byte
*/
char *gen_bytes(int size) {
	srand(time(0));
	char *dest = (char *)malloc(size);
	int nwords = size / 4;
	printf("%d\n", nwords);
	int buffer[nwords + 1];
	for (int i = 0; i <= nwords; i++ ){
		buffer[i] = rand();
		printf("%08x", buffer[i]);
	}
	printf("\n");
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




#endif




