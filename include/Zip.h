//
// Created by dashuai on 2022/4/3.
//

#ifndef BUPT_FINAL_ZIP_H
#define BUPT_FINAL_ZIP_H

//#include "Huffman.h"
#include "OutputBuffer.h"
#include "ZlibBuffer.h"
#include "INFO.h"
#include "InputBuffer.h"
#include "AC.h"

namespace ZIP {

const int32_t N = 64 * 1024;
OutputBuffer out_buf(N);// uint8_t

void pushIntoOutBuf(const uint8_t &c) {
    out_buf.push(c);
}

//Huffman H(N, pushIntoOutBuf);//  uint8_t  /// 哈夫曼编码之后送入输出数组
//
//void pushIntoH(const uint8_t &c) {
//    H.push(c);
//}

ZlibBuffer after_match_buffer(N, pushIntoOutBuf);/// 控制符、未压缩字符，合并后进行huffman编码


InfoStack AC_info_stack(N ,[](const auto x){std::cout<<"AC_info_Stack error\n";exit(0);});

long long pt = 0;

void deal_info(const char &c) {
    if (AC_info_stack.size() > 0) {
        auto info_head = AC_info_stack.get(0);
        if (info_head.start_pos() == pt) {
            after_match_buffer.push(128 + info_head.len);
        } else if (info_head.start_pos() < pt && pt <= info_head.end_pos) {

        } else {
            after_match_buffer.push(c);
        }
        if (info_head.end_pos == pt) {
            AC_info_stack.fastPopFront();
        }
    } else {
        after_match_buffer.push(c);
    }
    ++pt;
}

CircularQueue<char> AC_buffer(N, deal_info);
AC::AutoMaton<400> autoMaton(AC_info_stack);


void ac_match(const char &c) {
    autoMaton.match(c);
    AC_buffer.push(c);
}

InputBuffer input_buffer(N, ac_match);


void clear_all() {
    AC_buffer.clear_and_do();
    after_match_buffer.clear_and_do();
    //H.clear_and_do();
    out_buf.clear_and_do();
}

void compress(char input_file_name[], char output_file_name[]) {
    out_buf.setOutputFile(output_file_name);
    autoMaton.init(PatternStr::pattern_str);
    input_buffer.readData(input_file_name);
    clear_all();
}


}

namespace UNZIP {
const int32_t N = 64 * 1024;

OutputBuffer out_stream(N);

//HuffmanDecode HD = HuffmanDecode(N * 50, [&UM](const uint8_t &c) {// 这地方写的有点hack
//    UM.push(c);
//});

void pushIntoOutStream(const uint8_t &c) {
    if (c > 127) {
        for (const auto &it: PatternStr::pattern_str[c - 128]) {
            out_stream.push(it);
        }
    } else {
        out_stream.push(c);
    }
}

UnZlibBuffer UM(N * 10, pushIntoOutStream);


void uncompress(char target_file[], char output_file[]) {
    FILE *target = fopen(target_file, "r");

    out_stream.setOutputFile(output_file);

    int c;
    while ((c = getc(target)) != EOF) {
        UM.push(c);
    }
    out_stream.clear_and_do();
    fclose(target);
}

}
#endif //BUPT_FINAL_ZIP_H
