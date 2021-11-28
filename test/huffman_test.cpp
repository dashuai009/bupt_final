#include<bits/stdc++.h>

#include "Huffman.h"
#include "Log.h"

const int N = 64 << 10;
Buffer<uint8_t> input_char(N);
Huffman H(N);
HuffmanDecode HD(N);
int main() {
    std::string text = "asdfasdfasdfasdsdfgjkandfgiuahuih8r7q23yr,./,98687jhfjgkjbjhds546u2345o8975 7857I765SO7C5uxfcuyjhrfoyuashdbf8yhgiuzsddfhbu";
    text = "2";
    //Log(std::cout, std::hex);
    std::cout<<std::hex;
    HD.f = [&text](uint8_t x){
        static int cnt=0;
        if(x!=text[cnt++]){
            printf("error\n");
        }
        printf("t%c",x);
    };
    H.f = [](uint8_t x) {
        //Log(std::cout, x, ' ');
        //std::cout<<x<<' ';
        printf("%x ",x);
        HD.push(x);
    };
    input_char.f = [](uint8_t x) {
        H.push(x);
    };
    for(auto it:text){
        input_char.push(it);
    }
    input_char.clear_and_do();
    H.clear_and_do();
    return 0;
}