//
// Created by dashuai on 2022/3/24.
//

#ifndef BUPT_FINAL_COMPRESSBUFFER_H
#define BUPT_FINAL_COMPRESSBUFFER_H

#include <utility>
#include <zlib.h>
#include "PatternStr.h"



/*
 * 4 byte 0/1控制串的bit长度 n
 * 4bytes 待编码区域的byte长度 k。
 * (n+7)/8 比特的 0/1控制串
 * k bytes的 待编码字符。
 * */
class MatchBuffer:Buffer<uint8_t> {
    //using Buffer<uint8_t>::Buffer;
public:
    uint8_t *text_out = nullptr;

    /**
     * 压缩，每组输出大小 < 2*buffer_size
     * @param buffer_size  经过AC自动机匹配之后，分控制位流和字符流。两个数据流均为buffer_size一组。
     * @param f
     */
    MatchBuffer(uint32_t buffer_size, std::function<void(uint8_t)> f) : Buffer<uint8_t>(buffer_size,f) {
        text_out = new Bytef[buffer_size * 5];
    };

    ~MatchBuffer() {
        delete[] text_out;
    }

    template<typename T>
    void applyF(const T x, const std::function<void(uint8_t)> &f) {

    }
    void clear_and_do() override {
        uLongf zlib_size = BUFFER_SIZE * 5;
        auto res = compress2(text_out, &zlib_size, buffer, (unsigned long) buffer_offset,3);
        assert(res == Z_OK);
        for (int i = (sizeof(zlib_size) - 1) * 8; i >= 0; i -= 8) {
            f(zlib_size >> i);
        }
        for (int i = 0; i < zlib_size; ++i) {
            f(text_out[i]);
        }
        buffer_offset = 0;
    }

    void push(const uint8_t &c) {
        buffer[buffer_offset++] = c;//ctrl_bit == 1，普通字符，=0,模式串编号
        if (buffer_offset == BUFFER_SIZE) {
            clear_and_do();
        }
    }

};


class UnzipMatchBuffer : public Buffer<uint8_t> {
    //explicit UnzipMatchBuffer(uint32_t buffer_size) : Buffer<uint8_t>(buffer_size) {}
public:
    uint32_t textSize{};
    uint8_t *uncompressedBuffer;

    /**
     * 解压
     * @param size
     * @param f
     */
    UnzipMatchBuffer(int32_t size, std::function<void(uint8_t)> f) : Buffer(size, std::move(f)) {
        uncompressedBuffer = new uint8_t[size];
    }

    void clear_and_do() override {
        uLongf uncompressLen = BUFFER_SIZE;
        auto res = uncompress(uncompressedBuffer, &uncompressLen,
                              buffer + sizeof(uLongf), textSize);
        assert(res == Z_OK);

        for (int i = 0; i < uncompressLen; ++i) {
            f(uncompressedBuffer[i]);
        }
        textSize = 0;
        buffer_offset = 0;
    }

    void push(const uint8_t &c) override {
        buffer[buffer_offset++] = c;
        if (buffer_offset <= sizeof(uLongf)) {
            textSize = (textSize << 8) | c;
        } else if (buffer_offset == sizeof(uLongf) + textSize) {
            clear_and_do();
        }
    }
};

#endif //BUPT_FINAL_COMPRESSBUFFER_H
