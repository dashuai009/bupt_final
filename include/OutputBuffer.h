//
// Created by dashuai on 2022/3/20.
//
#include "Buffer.h"
#ifndef BUPT_FINAL_OUTPUTBUFFER_H
#define BUPT_FINAL_OUTPUTBUFFER_H

class OutputBuffer : public Buffer<uint8_t> {
    using Buffer<uint8_t>::Buffer;
private:
    FILE *output_ptr = nullptr;
public:
    void setOutFile(char file[]){
        output_ptr = fopen(file, "w");
    }
    ~OutputBuffer(){
        if(output_ptr){
            fclose(output_ptr);
            output_ptr = nullptr;
        }
    }

    void clear_and_do() override {
        fwrite(buffer, 1, buffer_offset, output_ptr);
        buffer_offset = 0;
    }

    void setOutputFile(char output_file_name[]){
        output_ptr = fopen(output_file_name,"wb");
        if(!output_ptr){
            std::cerr<<"Set output file error!\n";
        }
    }
};

#endif //BUPT_FINAL_OUTPUTBUFFER_H
