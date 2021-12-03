//
// Created by dashuai on 2021/11/28.
//

#ifndef BUPT_FINAL_READANDWRITEFILE_H
#define BUPT_FINAL_READANDWRITEFILE_H

#include "Buffer.h"

/**
 * read file and clear input_buf
 * @param input_file_name
 * @param input_buf
 */
void readInPutFile(char input_file_name[], Buffer<uint8_t> &input_buf) {
    FILE *input_ptr = fopen(input_file_name, "rb");
    if (!input_ptr) {
        std::cerr << "input_file error!\n";
        return;
    }
    // obtain file size:
    fseek(input_ptr, 0, SEEK_END);
    auto input_size = ftell(input_ptr);
    rewind(input_ptr);

    for (uint64_t i = 0; i < input_size / input_buf.BUFFER_SIZE; ++i) {
        fread(input_buf.buffer, 1, input_buf.BUFFER_SIZE, input_ptr);
        input_buf.buffer_offset = input_buf.BUFFER_SIZE;
        input_buf.clear_and_do();
        //Log(std::cout, i);
    }
    if (input_size % input_buf.BUFFER_SIZE > 0) {
        fread(input_buf.buffer, 1, input_size % input_buf.BUFFER_SIZE, input_ptr);
        input_buf.buffer_offset = input_size % input_buf.BUFFER_SIZE;
        input_buf.clear_and_do();
    }
    fclose(input_ptr);
}


struct OutBuffer : Buffer<uint8_t> {
    explicit OutBuffer(uint32_t size): Buffer<uint8_t>(size){

    }
    FILE *output_ptr = nullptr;

    void clear_and_do() override {
        fwrite(buffer, 1, buffer_offset, output_ptr);
        buffer_offset = 0;
    }
};

void writeOutFile(char output_file_name[], OutBuffer &out_buf) {
    FILE *output_ptr = fopen(output_file_name, "wb");
    out_buf.output_ptr = output_ptr;
}

#endif //BUPT_FINAL_READANDWRITEFILE_H
