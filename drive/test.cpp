#include <iostream>
#include <string>
#include <cstring>

#include "dma_io.h"

using namespace std;

#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())
#define xdma_h2c_0 "/dev/xdma0_h2c_0"

int main(){

    string tstr = "test";
    char *tstr2 = string_to_char_array(tstr);

    cout << tstr << endl;
    printf("%s\n", tstr2);
    //printf("%s\n", string_to_char_array(xdma_h2c_0));

    return 0;
}