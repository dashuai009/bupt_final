//
// Created by dashuai on 2022/3/24.
//

#ifndef BUPT_FINAL_ZLIBBUFFER_H
#define BUPT_FINAL_ZLIBBUFFER_H

#include <utility>
#include <zlib.h>

/*
 *
 * 将输入数据压缩，注意解压缩的buffer要足够大
 * */
class ZlibBuffer : Buffer<uint8_t> {
    //using Buffer<uint8_t>::Buffer;
public:
    uint8_t *text_out = nullptr;

    /**
     * 压缩，默认压缩级别是4
     * @param buffer_size  每buffer_size一组压缩
     * @param f
     */
    ZlibBuffer(int32_t buffer_size, const std::function<void(uint8_t)> &f) : Buffer<uint8_t>(buffer_size, f) {
        text_out = new Bytef[buffer_size * 5];
    };

    ~ZlibBuffer() {
        delete[] text_out;
    }

    void clear_and_do() override {
        uLongf zlib_size = BUFFER_SIZE * 5;
        auto res = compress2(text_out, &zlib_size, buffer, (unsigned long) buffer_offset, 4);
        assert(res == Z_OK);
        for (int i = (sizeof(zlib_size) - 1) * 8; i >= 0; i -= 8) {
            f(zlib_size >> i);
        }
        for (int i = 0; i < zlib_size; ++i) {
            f(text_out[i]);
        }
        buffer_offset = 0;
    }

    void push(const uint8_t &c) override {
        buffer[buffer_offset++] = c;
        if (buffer_offset == BUFFER_SIZE) {
            clear_and_do();
        }
    }

};


class UnZlibBuffer : public Buffer<uint8_t> {
    //explicit UnzipMatchBuffer(uint32_t buffer_size) : Buffer<uint8_t>(buffer_size) {}
public:
    uint32_t textSize{};
    uint8_t *uncompressedBuffer;

    /**
     * 解压
     * @param size
     * @param f
     */
    UnZlibBuffer(int32_t size, std::function<void(uint8_t)> f) : Buffer(size, std::move(f)) {
        uncompressedBuffer = new uint8_t[size * 5];
    }

    void clear_and_do() override {
        uLongf uncompressLen = BUFFER_SIZE * 5;
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

#endif //BUPT_FINAL_ZLIBBUFFER_H
