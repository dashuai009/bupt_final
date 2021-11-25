//
// Created by dashuai on 2021/11/20.
//
// huffman编码结果  512个int64_t(frequency)，一个int64_t表示紧跟之后的压缩文本的比特数，比特数/8个字节的压缩数据
#ifndef BUPT_FINAL_HUFFMAN_H
#define BUPT_FINAL_HUFFMAN_H
#ifndef  BUPT_FINAL_BUFFER_H
#include"Buffer.h"
#endif //BUPT_FINAL_BUFFER_H

#include<vector>
#include<queue>
#include<functional>
#include<algorithm>
#include "BitSet.h"

using C = uint32_t;
using CHAR = uint8_t;

struct Huffman : Buffer<CHAR> {
public:
    C totalCnt = 0;
    C char_cnt[512]{};
    using encoded_bits = BitSet<uint8_t>;
    encoded_bits bit[512];


    struct Huffman_node {
        Huffman_node *lson, *rson;
        CHAR c;

        Huffman_node(const CHAR &c, Huffman_node *lson, Huffman_node *rson) : lson(lson), rson(rson), c(c) {};
    } *root = nullptr;

    using P = std::pair<C, Huffman_node *>;

    struct myGreater : std::binary_function<P, P, bool> {
        bool operator()(const P &x, const P &y) const { return x.first > y.first; }
    };

    void clear_and_do() override {
        if (buffer_offset == 0) {
            return;
        }
        totalCnt = calc();
        CHAR tmp = 0;
        for (const C &i: char_cnt) {
            for (int j = 24; j >= 0; j -= 8) {
                tmp = ((unsigned int) i >> j) & 0xff;
                f(tmp);
            }
        }
        encoded_bits res;
        //res.resize(BUFFER_SIZE);
        for (int i = 0; i < buffer_offset; ++i) {
            const auto &code = bit[buffer[i]];//&！！！！
            res.append(code);
        }
        for (int i = 0; i < (res.Size >> 3); ++i) {
            f(res.bits[i]);
        }
        if (res.Size & 7) {
            uint8_t lastChar = (res.bits[res.Size >> 3] << (8 - (res.Size & 7)));
            f(lastChar);
        }
        // Log(std::cout,totalCnt,res.Size);
        memset(char_cnt, 0, sizeof(char_cnt));
        buffer_offset = 0;
    }

    void update_bit(Huffman_node *cur_node, encoded_bits cur_bits) {
        if (cur_node->lson || cur_node->rson) {
            auto new_bits = (cur_bits);
            new_bits.push_back(false);
            if (cur_node->lson) {
                update_bit(cur_node->lson, new_bits);
            }
            new_bits.pop_back();
            new_bits.push_back(true);
            if (cur_node->rson) {
                update_bit(cur_node->rson, new_bits);
            }
            new_bits.pop_back();
        } else {
            bit[cur_node->c] = (cur_bits);
        }
    }

    void push(const CHAR &x) override {
        char_cnt[x]++;
        buffer[buffer_offset++] = x;
        if (buffer_offset == BUFFER_SIZE) {
            clear_and_do();
        }
    }

    //建树，更新bit数组，返回总的编码长度(bit)
    C calc() {
        std::priority_queue<P, std::vector<P>, myGreater> Huffman_heap;
        for (int i = 0; i < 512; ++i) {
            if (char_cnt[i] > 0) {
                Huffman_heap.push(P{char_cnt[i], new Huffman_node(i, nullptr, nullptr)});
            }
        }
        C res = 0;
        root = new Huffman_node(0, Huffman_heap.top().second, nullptr);
        for (; Huffman_heap.size() > 1;) {
            auto top1 = Huffman_heap.top();
            Huffman_heap.pop();
            auto top2 = Huffman_heap.top();
            Huffman_heap.pop();
            auto new_top = P{top1.first + top2.first, root = new Huffman_node(0, top1.second, top2.second)};
            res += top1.first + top2.first;
            Huffman_heap.push(new_top);
        }
        auto tmp = encoded_bits();
        update_bit(root, tmp);
        return res;
    }
};

struct HuffmanDecode : public Huffman {

    void clear_and_do() override {
        //totalCnt = calc();
        auto cur_node = root;
        for (int i = sizeof(char_cnt); i < buffer_offset; ++i) {
            for (int j = 7; j >= 0 && (((i - sizeof(char_cnt) + 1)<<3) - j) <= totalCnt; --j) {
                if (__builtin_expect(buffer[i] & (1 << j), 1)) {//条件为真的概率大
                    cur_node = cur_node->rson;
                } else {
                    cur_node = cur_node->lson;
                }
                if (__builtin_expect(cur_node->lson == nullptr && cur_node->rson == nullptr, 0)) {
                    f(cur_node->c);
                    cur_node = root;
                }
            }
        }
        //Log(std::cout,buffer_offset);
        buffer_offset = 0;
    }

    void push(const CHAR &x) override {
        buffer[buffer_offset++] = x;
        if (buffer_offset <= sizeof(char_cnt)) {
            auto tail = buffer_offset - 1;
            auto i = tail / sizeof(C);
            char_cnt[i] = (char_cnt[i] << 8) | (C) (unsigned char) x;
            if (buffer_offset == sizeof(char_cnt)) {
                totalCnt = calc();
            }
        } else if (buffer_offset - sizeof(char_cnt) == (totalCnt + 7) / 8) {
            clear_and_do();
        }

    }
};

#endif //BUPT_FINAL_HUFFMAN_H
