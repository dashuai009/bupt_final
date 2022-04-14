//
// Created by dashuai on 2022/4/3.
//

#ifndef BUPT_FINAL_ZIP_H
#define BUPT_FINAL_ZIP_H

#include "Huffman.h"
#include "OutputBuffer.h"
#include "CompressBuffer.h"
#include "INFO.h"
#include "InputBuffer.h"

namespace ZIP {

const int32_t N = 64 * 1024;
OutputBuffer out_buf(N);// uint8_t

void pushIntoOutBuf(const uint8_t &c) {
    out_buf.push(c);
}

Huffman H(N, pushIntoOutBuf);//  uint8_t  /// 哈夫曼编码之后送入输出数组

void pushIntoH(const uint8_t &c) {
    H.push(c);
}

MatchBuffer after_match_buffer(N, pushIntoH);/// 控制符、未压缩字符，合并后进行huffman编码


InfoStack AC_info_stack(N * 3);

long long pt = 0;

void deal_info(const char &c) {
    if (AC_info_stack.size() > 0) {
        auto info_head = AC_info_stack.get(0);
        if (info_head.start_pos() == pt) {
            after_match_buffer.push(false, info_head.len);
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
}

CircularQueue<char> AC_buffer(N, deal_info);
AC::AutoMaton autoMaton(AC_info_stack);


void ac_match(const char &c) {
    autoMaton.match(c);
    AC_buffer.push(c);
}

InputBuffer input_buffer(N, ac_match);


void clear_all() {
    AC_buffer.clear_and_do();
    after_match_buffer.clear_and_do();
    H.clear_and_do();
    out_buf.clear_and_do();
}

void compress(char input_file_name[], char output_file_name[]) {
    out_buf.setOutputFile(output_file_name);
    autoMaton.build_from_pattern();
    input_buffer.readData(input_file_name);
    clear_all();
}


}

#endif //BUPT_FINAL_ZIP_H
