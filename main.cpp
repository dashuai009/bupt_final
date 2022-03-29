#include <bits/stdc++.h>
#include "include/AC.h"
#include "include/Buffer.h"
#include "include/CircularQueue.h"
#include "include/CompressBuffer.h"
#include "include/Huffman.h"
#include "include/Log.h"
#include "include/SetOptions.h"
#include "include/OutputBuffer.h"
#include "include/InputBuffer.h"
#include "include/INFO.h"

const int32_t N = 128 * 1024;

void zip(char input_file_name[], char output_file_name[]) {
    OutputBuffer out_buf{N};
    out_buf.setOutputFile(output_file_name);

/// 哈夫曼编码之后送入输出数组
    Huffman H = Huffman(N, [&out_buf](const auto &c)constexpr {
        out_buf.push(c);
    });

/// 控制符、未压缩字符，合并后进行huffman编码
    MatchBuffer after_match_buffer(N, [&H](const uint8_t &c) constexpr {
        H.push(c);
    });

    InfoStack AC_info_stack = InfoStack(N * 10, [](auto x)constexpr {
        //std::cout<<"xxxxxx";
    });

    long long pt = 0;
    CircularQueue<char> AC_buffer(N, [&AC_info_stack, &after_match_buffer, &pt](const char &c) constexpr {
        if (AC_info_stack.size() > 0) {
            auto info_head = AC_info_stack.get(0);
            if (info_head.start_pos() == pt) {
                after_match_buffer.push(false, info_head.len);
            } else if (info_head.start_pos() < pt && pt <= info_head.end_pos) {

            } else {
                after_match_buffer.push(true, c);
            }
            if (info_head.end_pos == pt) {
                AC_info_stack.start_pos++;
                if(AC_info_stack.start_pos >= AC_info_stack.BUFFER_SIZE){
                    AC_info_stack.start_pos = 0;
                }
            }
        } else {
            after_match_buffer.push(true, c);
        }
        ++pt;
    });

    AC::AutoMaton autoMaton;
    autoMaton.build_from_pattern();

    InputBuffer input_buffer(N, [&autoMaton, &AC_info_stack, &AC_buffer](const char &c)constexpr {
        AC_info_stack.AC_match(c, autoMaton.root);
        AC_buffer.push(c);
    });
    ///开始压缩
    input_buffer.readData(input_file_name);

    ///结束压缩
    AC_buffer.clear_and_do();
    after_match_buffer.clear_and_do();
    H.clear_and_do();
    out_buf.clear_and_do();
}


void unzip(char target_file[], char config_file[], char output_file[]) {
    FILE *target = fopen(target_file, "r");

    OutputBuffer out_stream(N);
    out_stream.setOutputFile(output_file);
    UnzipMatchBuffer UM(N * 10, [&out_stream](const uint8_t &c) {
        out_stream.push(c);
    });
    HuffmanDecode HD = HuffmanDecode(N * 50, [&UM](const uint8_t &c) {// 这地方写的有点hack
        UM.push(c);
    });
    int c;
    while ((c = getc(target)) != EOF) {
        HD.push(c);
    }
    //UM.clear_and_do();
    out_stream.clear_and_do();
    fclose(target);
}


int main(int argc, char *argv[]) {
    auto beginTime = std::chrono::system_clock::now();
    set_options(argc, argv);
    AC::readPatternStr(config_file);
    if (mode == 0) {
        zip(input_file, output_file);
    } else {
        unzip(input_file, config_file, output_file);
    }
    auto endTime = std::chrono::system_clock::now();
    auto dur = std::chrono::duration<double,std::milli>(endTime - beginTime);
    std::cout <<"cost "<< dur.count() << " ms\n";
    return 0;
}
