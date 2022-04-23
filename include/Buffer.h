//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_BUFFER_H
#define BUPT_FINAL_BUFFER_H

#include <functional>
#include "Log.h"
/**
 * @brief
 *
 * @tparam T：buffer中的数据的类
 * f 处理函数，输入参数为T的函数
 */
template<typename T>
class Buffer {
public:
    int32_t BUFFER_SIZE = 64 * 1024;
    T *buffer;
    int32_t buffer_offset = 0;
    std::function<void(T)> f;

     explicit Buffer(int32_t size) : BUFFER_SIZE(size) {/*rest*/
        buffer = new T[size];
    }
     Buffer(int32_t size,const std::function<void(T)> f):BUFFER_SIZE(size),f(f){
         buffer = new T[size];
    }

    ~Buffer() {
        delete [] buffer;
    }

    virtual void clear_and_do() {
        for (int32_t i = 0; i < buffer_offset; ++i) {
            f(buffer[i]);
        }
        buffer_offset = 0;
    };

    virtual void push(const T &c) {
        buffer[buffer_offset++] = c;
        if (buffer_offset == BUFFER_SIZE) {//TODO think
            clear_and_do();
        }
    }
};

#endif //BUPT_FINAL_BUFFER_H