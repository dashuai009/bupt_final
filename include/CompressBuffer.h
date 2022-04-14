//
// Created by dashuai on 2022/3/24.
//

#ifndef BUPT_FINAL_COMPRESSBUFFER_H
#define BUPT_FINAL_COMPRESSBUFFER_H

#include <utility>
#include <zlib.h>
#include "BitSet.h"

template<typename T>
void applyF(const T x, const std::function<void(uint8_t)> &f) {
    for (int i = (sizeof(T) - 1) * 8; i >= 0; i -= 8) {
        f(x >> i);
    }
}

/*
 * 4 byte 0/1控制串的bit长度 n
 * 4bytes 待编码区域的byte长度 k。
 * (n+7)/8 比特的 0/1控制串
 * k bytes的 待编码字符。
 * */
class MatchBuffer {
public:
    uint32_t BUFFER_SIZE = 0;
    uint32_t buffer_offset = 0;
    std::function<void(uint8_t)> f = nullptr;
    BitSet<uint8_t> ctrl_flow{};
    uint8_t *text_buf = nullptr;
    uint8_t *text_out = nullptr;
    //uint8_t *text_test = nullptr;

    /*
    explicit MatchBuffer(uint32_t buffer_size) : BUFFER_SIZE(buffer_size) {
        text_buf = new uint8_t[buffer_size];
        text_out = new uint8_t[buffer_size * 2];
        //text_test = new uint8_t[buffer_size];
    }
    */

    /**
     * 压缩，每组输出大小 < 2*buffer_size
     * @param buffer_size  经过AC自动机匹配之后，分控制位流和字符流。两个数据流均为buffer_size一组。
     * @param f
     */
    MatchBuffer(uint32_t buffer_size, std::function<void(uint8_t)> f) : BUFFER_SIZE(buffer_size), f(std::move(f)) {
        text_buf = new Bytef[buffer_size];
        text_out = new Bytef[buffer_size * 5];
    };

    ~MatchBuffer() {
        delete[] text_buf;
        delete[] text_out;
        //delete[] text_test;
    }


    void clear_and_do() {
        //Log(std::cout, ctrl_flow.Size);#define Z_BUF_ERROR    (-5)

        //Log(std::cout, need_to_huffman.size());
        uLongf zlib_size = BUFFER_SIZE * 5;
        auto res = compress(text_out, &zlib_size, text_buf, (unsigned long)buffer_offset);
        //std::cout<<"ZIP \torigin size:"<<buffer_offset<<" zipped size:"<<zlib_size<<'\n';
        assert(res == Z_OK);
        //Log(std::cout, "origin", buffer_offset);
        applyF(ctrl_flow.Size, f);//4 bytes ==buffer_offset
        applyF((uint32_t)zlib_size, f);//4 bytes
        assert(sizeof((uint32_t)zlib_size) == 4);
        for (auto it: ctrl_flow.bits) {
            f(it);
        }
        for (int i = 0; i < zlib_size; ++i) {
            f(text_out[i]);
        }
        Log(std::cout, "crtl_flow_size:", ctrl_flow.Size, "bits", (ctrl_flow.Size + 7) / 8, "bytes");
        Log(std::cout, "zlib size:", zlib_size);
        ctrl_flow.clear();
        buffer_offset = 0;
        //memset(text_out,0,sizeof(text_out));
    }

    void push(const bool &ctrl_bit, const uint8_t &c) {
        ctrl_flow.push_back(ctrl_bit);
        text_buf[buffer_offset++] = c;
        if (ctrl_flow.Size == BUFFER_SIZE) {
            clear_and_do();
        }
    }

};


class UnzipMatchBuffer : public Buffer<uint8_t> {
    //explicit UnzipMatchBuffer(uint32_t buffer_size) : Buffer<uint8_t>(buffer_size) {}
public:
    uint32_t ctrl_flow_size = 0;
    uint32_t info_size = 0;
    BitSet<uint8_t> ctrl_flow;
    uint8_t *uncompressedBuffer;

    /**
     * 解压
     * @param size
     * @param f
     */
    UnzipMatchBuffer(int32_t size, std::function<void(uint8_t)> f):Buffer(size,std::move(f)){
        uncompressedBuffer = new uint8_t[size];
    }

    void clear_and_do() override {
        for (int i = 8; i < 8 + (ctrl_flow_size + 7) / 8; ++i) {
            ctrl_flow.push_back(buffer[i]);
        }
        ctrl_flow.Size = ctrl_flow_size;
        uLongf uncompressLen = BUFFER_SIZE;
        auto res = uncompress(uncompressedBuffer, &uncompressLen,
                   buffer + 8 + (ctrl_flow_size + 7) / 8, info_size);
        assert(res == Z_OK);
        assert(ctrl_flow_size == uncompressLen);
        Log(std::cout, "crtl_flow_size:", ctrl_flow.Size, "bits", (ctrl_flow.Size + 7) / 8, "bytes");
        Log(std::cout, "待编码字符流：", info_size, "bytes");
        //std::cout<<"unzip \torigin size:"<<uncompressLen<<" zipped size:"<<info_size<<"\n\n";

        uint8_t *ptr = uncompressedBuffer;
        ctrl_flow.forEach([this, &ptr, &uncompressLen](const bool x) {
            if (x) {
                this->f(*ptr);
            } else {
                for (const auto &it: PatternStr::pattern_str[*ptr]) {
                    this->f(it);
                }
            }
            assert(ptr - uncompressedBuffer <= uncompressLen);
            ++ptr;
        });
        ctrl_flow_size = 0;
        info_size = 0;
        ctrl_flow.clear();
        buffer_offset = 0;
    }

    void push(const uint8_t &c) override {
        buffer[buffer_offset++] = c;
        if (buffer_offset <= 4) {
            ctrl_flow_size = (ctrl_flow_size << 8) | c;
        } else if (buffer_offset <= 8) {
            info_size = (info_size << 8) | c;
        } else if (buffer_offset == 8 + (ctrl_flow_size + 7) / 8 + info_size) {
            clear_and_do();
        }
    }
};

#endif //BUPT_FINAL_COMPRESSBUFFER_H
