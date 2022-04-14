//
// Created by dashuai on 2022/3/27.
//

#include "gtest/gtest.h"
#include "include/AC.h"
#include "include/InputBuffer.h"
#include "include/INFO.h"
#include "include/CircularQueue.h"
#include "include/CompressBuffer.h"
#define N 667

class ACTest:public ::testing::Test{
protected:
    void SetUp()override{
        inStr = outStr = "";
        char config[] = "data/defaultConfig.txt";
        AC::readPatternStr(config);
        autoMaton.build_from_pattern();

        char file[] = "data/test_in.txt";
        i.readData(file);
        i.clear_and_do();
        AC_buffer.clear_and_do();
        after_match_buffer.clear_and_do();
    }
    std::string outStr;
    std::string inStr;

    UnzipMatchBuffer UM = UnzipMatchBuffer(N * 10, [&](const uint8_t &c) {
        outStr +=(char)c;
    });

/// 控制符、未压缩字符，合并后进行huffman编码
    MatchBuffer after_match_buffer = MatchBuffer(N, [&](const uint8_t &c) constexpr {
        UM.push(c);
    });

    InfoStack AC_info_stack = InfoStack(N * 3 ,[](auto x)constexpr {
        std::cout<<"xxxxxxxxxxxxxxxxxx \n";
    });


    long long pt = 0;
    CircularQueue<char> AC_buffer = CircularQueue<char>(N, [&](const char &c) constexpr {
        if (AC_info_stack.size() > 0) {
            auto info_head = AC_info_stack.get(0);
            if (info_head.start_pos() == pt) {
                after_match_buffer.push(false, info_head.len);
                assert(info_head.len < 128);
            } else if (info_head.start_pos() < pt && pt <= info_head.end_pos) {

            } else {
                after_match_buffer.push(true, c);
            }
            if (info_head.end_pos == pt) {
                AC_info_stack.fastPopFront();
            }
        } else {
            after_match_buffer.push(true, c);
        }
        ++pt;
    });

    AC::AutoMaton autoMaton;

    InputBuffer i = InputBuffer(N,[&](auto c){
        AC_info_stack.AC_match(c, autoMaton.root);
        AC_buffer.push(c);
        inStr += (char)c;
    });

};

TEST_F(ACTest,Fas) {
    for(int i=0;i<inStr.length();++i){
        if(i<outStr.length()){
            //EXPECT_EQ(inStr[i], outStr[i])<<i;
        }
    }
    EXPECT_EQ(inStr.length(),outStr.length());
    EXPECT_EQ(inStr,outStr);
}