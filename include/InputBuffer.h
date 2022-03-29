//
// Created by dashuai on 2022/3/20.
//
#include "Buffer.h"

#ifndef BUPT_FINAL_INPUTBUFFER_H
#define BUPT_FINAL_INPUTBUFFER_H

class InputBuffer : public Buffer<uint8_t> {
    using Buffer<uint8_t>::Buffer;
private:
    FILE *input_ptr;

public:
    void setInputFile(FILE* f){
        input_ptr = f;
    }

    void readData(char input_file_name[]) {
        input_ptr = fopen(input_file_name, "rb");
        if (!input_ptr) {
            std::cerr << "input_file error!\n";
            return;
        }
        // obtain file size:
        fseek(input_ptr, 0, SEEK_END);
        auto input_size = ftell(input_ptr);
        rewind(input_ptr);

        for (uint64_t i = 0; i < input_size / BUFFER_SIZE; ++i) {
            fread(buffer, 1, BUFFER_SIZE, input_ptr);
            buffer_offset = BUFFER_SIZE;
            clear_and_do();
        }
        if (input_size % BUFFER_SIZE > 0) {
            fread(buffer, 1, input_size % BUFFER_SIZE, input_ptr);
            buffer_offset = input_size % BUFFER_SIZE;
            clear_and_do();
        }
        fclose(input_ptr);
    }

};

#endif //BUPT_FINAL_INPUTBUFFER_H
