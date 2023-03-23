#ifndef __UTILS_H
#define __UTILS_H



#define printhex(chars, size) for (int i = 0; i < size; i++) printf("%02hhx", chars[i])
#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define int_from_bytes(bytes) int((unsigned char)bytes[0] | (unsigned char)bytes[1] << 8 | (unsigned char)bytes[2] << 16 | (unsigned char)bytes[3] << 24)

char *gen_bytes(int size);
char *bytes_from_int(int val);




#endif




