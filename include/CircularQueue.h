//
// Created by dashuai on 2021/11/26.
//

#ifndef BUPT_FINAL_CIRCULARQUEUE_H
#define BUPT_FINAL_CIRCULARQUEUE_H

/**
 * @brief
 *
 * @tparam F 回调函数，输入参数为char c的函数
 */
template<typename T>
class CircularQueue {
public:
    size_t BUFFER_SIZE = 64 * 4096;
    T *buffer = nullptr;
    int start_pos = 0;
    int end_pos = 0;
    std::function<void(T)> f = nullptr;

     CircularQueue(size_t size) : BUFFER_SIZE(size) {
        buffer = new T[BUFFER_SIZE];
    };
    CircularQueue(size_t size,const std::function<void(T)> & f):BUFFER_SIZE(size),f(f){
        buffer = new T[BUFFER_SIZE];
    }

    ~CircularQueue() {
        delete [] buffer;
    }

    void clear() {
        end_pos = start_pos;
    }

    void inc(int &pos) {
        ++pos;
        if (pos >= BUFFER_SIZE) {
            pos = 0;
        }
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
        buffer[end_pos] = c;
        inc(end_pos);
        if (this->size() >= (BUFFER_SIZE >> 1)) {//TODO think
            f(buffer[start_pos]);
            inc(start_pos);
        }
    }

    int size() {
        return (end_pos >= start_pos) ? end_pos - start_pos : BUFFER_SIZE - start_pos + end_pos;
    }

    //const T &back() {//TODO error
    //    return buffer[end_pos - 1];
    //}

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

#endif //BUPT_FINAL_CIRCULARQUEUE_H
