//
// Created by dashuai on 2021/11/20.
//
// huffman编码结果  512个int64_t(frequency)，一个int64_t表示紧跟之后的压缩文本的比特数，比特数/8个字节的压缩数据
#ifndef BUPT_FINAL_HUFFMAN_H
#define BUPT_FINAL_HUFFMAN_H
#define C int32_t
#ifndef  BUPT_FINAL_BUFFER_H
#include"Buffer.h"
#endif //BUPT_FINAL_BUFFER_H

#include<vector>
#include<queue>
#include<functional>
#include<algorithm>

struct Huffman : Buffer<char> {
public:
    C totalCnt = 0;
    C char_cnt[512]{};
    using encoded_bits = std::vector<char>;
    encoded_bits bit[512];


    struct Huffman_node {
        Huffman_node *lson, *rson;
        char c;

        Huffman_node(const char &c, Huffman_node *lson, Huffman_node *rson) : lson(lson), rson(rson), c(c) {};
    } *root = NULL;

    using P = std::pair<C, Huffman_node *>;

    struct myGreater : std::binary_function<P, P, bool> {
        bool operator()(const P &x, const P &y) const { return x.first > y.first; }
    };

    void clear_and_do() override {
        if (buffer_offset == 0) {
            return;
        }
        totalCnt = calc();
        //Log(std::cout, tot_bits);
        char tmp = 0;
        for (int i = 0; i < 512; ++i) {
            for (int j = 3; j >= 0; --j) {
                tmp = ((unsigned int) char_cnt[i] >> (j * 8)) & 0xff;
                f(tmp);
            }
        }
        tmp = 0;
        encoded_bits res;
        //res.resize(BUFFER_SIZE);
        for (int i = 0; i < buffer_offset; ++i) {
            auto &code = bit[(unsigned char) buffer[i]];//&！！！！
            res.insert(res.end(), code.begin(), code.end());
            //std::copy(code.begin(), code.end(), std::back_inserter(res));
        }
        int tmp_offset = 7;
        for (auto &b: res) {
            tmp |= (b << tmp_offset);
            --tmp_offset;
            if (tmp_offset == -1) {
                tmp_offset = 7;
                f(tmp);
                tmp = 0;
            }
        }
        if (tmp_offset != 7) {
            f(tmp);
        }
        memset(char_cnt, 0, sizeof(char_cnt));
        buffer_offset = 0;
    }

    void update_bit(Huffman_node *cur_node, encoded_bits cur_bits) {
        if (cur_node->lson || cur_node->rson) {
            auto new_bits = (cur_bits);
            new_bits.push_back(0);
            if (cur_node->lson) {
                update_bit(cur_node->lson, new_bits);
            }
            new_bits.pop_back();
            new_bits.push_back(1);
            if (cur_node->rson) {
                update_bit(cur_node->rson, new_bits);
            }
        } else {
            bit[(int) (unsigned char) cur_node->c] = (cur_bits);
        }
    }

    //建树，更新bit数组，返回总的编码长度(bit)
    void push(const char &x) override {
        char_cnt[(int) (unsigned char) x]++;
        buffer[buffer_offset++] = x;
        if (buffer_offset == BUFFER_SIZE) {
            clear_and_do();
        }
    }

    C calc() {
        std::priority_queue<P, std::vector<P>, myGreater> Huffman_heap;
        for (int i = 0; i < 512; ++i) {
            if (char_cnt[i] > 0) {
                Huffman_heap.push(P{char_cnt[i], new Huffman_node(char(i), NULL, NULL)});
            }
        }
        C res = 0;
        for (; Huffman_heap.size() > 1;) {
            auto top1 = Huffman_heap.top();
            Huffman_heap.pop();
            auto top2 = Huffman_heap.top();
            Huffman_heap.pop();
            auto new_top = P{top1.first + top2.first, root = new Huffman_node(0, top1.second, top2.second)};
            res += top1.first + top2.first;
            Huffman_heap.push(new_top);
        }
        if (root == NULL) {
            root = new Huffman_node(0, Huffman_heap.top().second, NULL);
        }
        auto tmp = encoded_bits();
        update_bit(root, tmp);
        return res;
    }
};

struct HuffmanDecode : public Huffman {

    void clear_and_do() override {
        totalCnt = calc();
        auto cur_node = root;
        for (int i = sizeof(char_cnt); i < buffer_offset; ++i) {
            for (int j = 7; j >= 0 && ((i - sizeof(char_cnt) + 1) * 8 - j) <= totalCnt; --j) {
                if (buffer[i] & (1 << j)) {
                    cur_node = cur_node->rson;
                } else {
                    cur_node = cur_node->lson;
                }
                if (cur_node->lson == nullptr && cur_node->rson == nullptr) {
                    f(cur_node->c);
                    cur_node = root;
                }
            }
        }
        buffer_offset = 0;
    }

    void push(const char &x) override {
        buffer[buffer_offset++] = x;
        if (buffer_offset <= sizeof(char_cnt)) {
            auto tail = buffer_offset - 1;
            auto i = tail / sizeof(C);
            //auto j = tail % sizeof(C);
            char_cnt[i] = (char_cnt[i]<<8) | (C)(unsigned char)x;
            //char_cnt[i] &= (-1 ^ (0xff << ((sizeof(C) - j - 1) * 8)));
            //char_cnt[i] |= (((C)(unsigned char) buffer[tail]) << ((sizeof(C) - 1 - j) * 8));
            if (buffer_offset == sizeof(char_cnt)) {
                totalCnt = calc();
            }
        } else if (buffer_offset - sizeof(char_cnt) == (totalCnt + 7) / 8) {
            clear_and_do();
        }

    }
};

#endif //BUPT_FINAL_HUFFMAN_H
