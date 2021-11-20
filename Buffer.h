//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_BUFFER_H
#define BUPT_FINAL_BUFFER_H
#include <functional>

#ifdef  BUPT_FINAL_LOG_H
#include "Log.h"
#endif

/**
 * @brief
 *
 * @tparam T：buffer中的数据的类
 * f 处理函数，输入参数为T的函数
 */
template<typename T>
class Buffer {
public:
    static const int BUFFER_SIZE = 320*1024*10;
    T buffer[BUFFER_SIZE];
    int buffer_offset = 0;
    std::function<void(T &)> f;

    Buffer(std::function<void(T &)> f) : f(f) {};

    Buffer() : f([](T &x) {}) {};

    virtual void clear_and_do() {
        for (int i = 0; i < buffer_offset; ++i) {
            f(buffer[i]);
        }
        buffer_offset = 0;
    }

    virtual  void push(const T &c) {
        buffer[buffer_offset++] = c;
        if (buffer_offset == BUFFER_SIZE) {//TODO think
            clear_and_do();
        }
    }
};

/**
 * @brief
 *
 * @tparam F 回调函数，输入参数为char c的函数
 */
template<typename T>
struct Buffer2 {
    static const int BUFFER_SIZE = 4096*10;
    T buffer[BUFFER_SIZE];
    int start_pos = 0;
    int end_pos = 0;
    std::function<void(T &)> f;

    Buffer2(std::function<void(T &)> f) : f(f) {};

    Buffer2() : f([](T &x) {}) {};

    void clear() {
        end_pos = start_pos;
    }

    void clear_and_do() {
        if (end_pos >= start_pos) {
            for (int i = start_pos; i < end_pos; ++i) {
                f(buffer[i]);
            }
        } else {
            for (int i = start_pos; i < BUFFER_SIZE; ++i) {
                f(buffer[i]);
            }
            for (int i = 0; i < end_pos; ++i) {
                f(buffer[i]);
            }
        }
        this->clear();
    }

    void push(const T &c) {
        buffer[end_pos++] = c;
        if (end_pos == BUFFER_SIZE) {
            end_pos = 0;
        }
        if (this->size() >= BUFFER_SIZE - 10) {//TODO think
            int med = (start_pos + BUFFER_SIZE / 2) % BUFFER_SIZE;
            if (med > start_pos) {
                for (int i = start_pos; i < med; ++i) {
                    f(buffer[i]);
                }
            } else {
                for (int i = start_pos; i < BUFFER_SIZE; ++i) {
                    f(buffer[i]);
                }
                for (int i = 0; i < med; ++i) {
                    f(buffer[i]);
                }
            }
            start_pos = med;
        }
    }

    int size() {
        return (end_pos >= start_pos) ? end_pos - start_pos : BUFFER_SIZE - start_pos + end_pos;
    }

    const T &back() {//TODO error
        return buffer[end_pos - 1];
    }

    const T &get(const int &pos) {//TODO try catch
        if (pos < 0) {
            return buffer[end_pos + pos < 0 ? end_pos + pos + BUFFER_SIZE : end_pos + pos];
        } else {
            return buffer[start_pos + pos >= BUFFER_SIZE ? start_pos + pos - BUFFER_SIZE : start_pos + pos];
        }
    }

    void pop_back(const int &n) {//TODO error
        end_pos = end_pos - n;
        if (end_pos < 0) {
            end_pos += BUFFER_SIZE;
        }

    }
};

#endif //BUPT_FINAL_BUFFER_H