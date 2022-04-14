//
// Created by dashuai on 2022/3/22.
//

#ifndef BUPT_FINAL_INFO_H
#define BUPT_FINAL_INFO_H

#include "CircularQueue.h"
#include "PatternStr.h"

struct INFO {
    long long end_pos;
    int forward;
    int len;

    [[nodiscard]] uint64_t length() const {
        return forward == 0 ? PatternStr::pattern_str[len].length() : len;
    }

    [[nodiscard]] long long start_pos() const {
        return end_pos + 1 - length();
    }
};

bool info_include(const INFO &x, const INFO &y) {//已经满足x.pt>=y.pt，满足”起始位置更靠前“则 x 包括了 y。
    return x.start_pos() <= y.start_pos();
};

class InfoStack : public CircularQueue<INFO> {
    using CircularQueue<INFO>::CircularQueue;
public:
    void clear_and_do() = delete;
    void push_st(const INFO &new_info) {
        int cnt_pop = 0;
        int sz = size();
        const auto new_info_start_pos = new_info.start_pos();
        while (sz > cnt_pop && new_info_start_pos <= get(-cnt_pop - 1).start_pos()) {
            ++cnt_pop;
        }
        if (size() == cnt_pop || new_info_start_pos > get(-cnt_pop - 1).end_pos) {//栈空，或不冲突
            pop_back(cnt_pop);
            push(new_info);
        } else if (size() > cnt_pop && new_info.length() > get(-cnt_pop - 1).length() &&
                   new_info_start_pos > get(-cnt_pop - 1).start_pos()) {//栈不空，冲突，若更好的
            pop_back(cnt_pop + 1);
            push(new_info);
        }
    }

};

#endif //BUPT_FINAL_INFO_H
