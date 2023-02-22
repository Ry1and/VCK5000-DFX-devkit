#include <iostream>
#include <string>
#include <cstring>

#include "dma_io.h"

using namespace std;

#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define xdma_h2c_0 "/dev/xdma0_h2c_0"


int returnint(char *outbuf) {
    printhex(outbuf);
    return (int)*outbuf;
}



int main(){

    string tstr = "tesrererer";
    char *tstr2 = string_to_char_array(tstr);
    long long *tint = (long long *)tstr2;



    cout << tstr << endl;
    printf("size: %lu\n", tstr.length());
    printf("size: %lu\n", sizeof(tstr2));
    printf("size: %lu\n", sizeof(*tint));
    printhex(tstr2);

    printf("%d", returnint(tstr2));

    //printf("%s\n", string_to_char_array(xdma_h2c_0));

    return 0;
}