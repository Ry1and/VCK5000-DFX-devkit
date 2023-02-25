#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>

#include "utils.h"

using namespace std;



#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define xdma_h2c_0 "/dev/xdma0_h2c_0"




int returnint(char *outbuf) {
    printhex(outbuf);
    return (int)*outbuf;
}



int main(){

    //string tstr = "test";
    //char *buf = (char *)malloc(16);
    //char *tstr2 = string_to_char_array(tstr);
    //gen_bytes(buf, 10);
    //char t[4]= "zxv";
    // int k = 0;
    // printf("%x\n", 'z');
    // printf("%x\n", 'x');
    // printf("%x\n", 'v');
    // printf("%x\n", 'a');
    //printf("%x\n", int_from_bytes(t));
    //strcpy(buf, tstr.c_str());
    
    // char *buf = (char *)malloc(4);

    // gen_bytes(buf, 4);
    // gen_bytes(buf, 4);
    // gen_bytes(buf, 4);
    // gen_bytes(buf, 4);

    //long *tint = (long *)buf;

   
    // char *buf = (char *)malloc(4);
    // int test = 0x9;

    // memcpy(buf, &test, 4);

    // printf("%x\n", int_from_bytes(buf));


    // timespec *tp = new timespec();
    // clock_gettime(CLOCK_MONOTONIC,tp);
    
    // printf("%ld\n", tp->tv_nsec);
    // usleep(1);
    // clock_gettime(CLOCK_MONOTONIC,tp);
    // printf("%ld\n", tp->tv_nsec);

    int test = 0x12345678;
    int buf = 0;
    memcpy(&buf, &test, 4);
    printf("%03x\n", buf);
    buf = 0;
    memcpy(&buf, &test, 3);
    printf("%x\n", buf);
    buf = 0;
    memcpy(&buf, &test, 2);
    printf("%x\n", buf);
    buf = 0;
    memcpy(&buf, &test, 1);
    printf("%x\n", buf);
    buf = 0;


    // cout << tstr << endl;
    // printf("size: %lu\n", tstr.length());
    // printf("size: %lu\n", strlen(buf));
    // printf("size: %lu\n", sizeof(*tint));
    // printhex(buf);


    //printf("%s\n", string_to_char_array(xdma_h2c_0));

    return 0;
}