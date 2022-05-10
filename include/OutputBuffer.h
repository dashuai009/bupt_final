//
// Created by dashuai on 2022/3/20.
//
#ifndef BUPT_FINAL_OUTPUTBUFFER_H
#define BUPT_FINAL_OUTPUTBUFFER_H

#include "Buffer.h"

class OutputBuffer : private Buffer<uint8_t> {
    using Buffer<uint8_t>::Buffer;
private:
    FILE *output_ptr = nullptr;
public:
    ~OutputBuffer() {
        if (output_ptr) {
            fclose(output_ptr);
            output_ptr = nullptr;
        }
    }

    void clear_and_do() override {
        fwrite(buffer, 1, buffer_offset, output_ptr);
        buffer_offset = 0;
    }

    void setOutputFile(const char output_file_name[]) {
        output_ptr = fopen(output_file_name, "wb");
        if (!output_ptr) {
            Log(std::cerr, "Set output file error!\n");
        }
    }

    void push(const uint8_t &x) override{
        Buffer<uint8_t>::push(x);
    }

    void push(uint8_t *a, int len) {
        while (buffer_offset + len >= BUFFER_SIZE) {
            memcpy(buffer + buffer_offset, a, BUFFER_SIZE - buffer_offset);
            a += BUFFER_SIZE - buffer_offset;
            len -= BUFFER_SIZE - buffer_offset;
            buffer_offset = BUFFER_SIZE;
            clear_and_do();
        }
        if (len > 0) {
            memcpy(buffer + buffer_offset, a, len);
            buffer_offset += len;
        }
    }

    void close() {
        fclose(output_ptr);
        output_ptr = nullptr;
    }
};

#endif //BUPT_FINAL_OUTPUTBUFFER_H
