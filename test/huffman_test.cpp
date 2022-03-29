#include "gtest/gtest.h"
#include "include/InputBuffer.h"
#include "include/Huffman.h"
#define N 10240

class HuffmanTest:public ::testing::Test{
protected:
    void SetUp()override{
        char file[] = "data/test_in1.txt";
        i.readData(file);
        i.clear_and_do();
        H.clear_and_do();
    }
    std::string ss;
    std::string dd;
    HuffmanDecode HD = HuffmanDecode(N,[&](auto x){
        ss += x;
    });


    Huffman H = Huffman(N,[&](auto x){
        HD.push(x);
    });

    InputBuffer i = InputBuffer(N,[&](auto x){
        H.push(x);
        dd += x;
    });
};

TEST_F(HuffmanTest,Fas) {
EXPECT_EQ(ss, dd);
}