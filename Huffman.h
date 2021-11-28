//
// Created by dashuai on 2021/11/20.
//
// huffman编码结果  5部分
// 0. 4byte,编码结果的bit数n。（第5部分）
// 1. 1byte 字符集大小m, 0表示有m=1个字符，1表示有m=2个，。。。，255表示有m=256个字符。。。空集应该不输出huffman编码
// 2. 2byte,k= dlr_bit_size
// 3. k个bit，表示先根遍历结果，m个bit,0表示非叶子节点，1表示对应字符是叶子节点。
// 4. m个字符，表示字符集中的字符
// 5. (n+7)/8个byte,编码结果。
#ifndef BUPT_FINAL_HUFFMAN_H
#define BUPT_FINAL_HUFFMAN_H
#ifndef  BUPT_FINAL_BUFFER_H

#include "Buffer.h"

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
    C totalChar = 0;
    C totalBit = 0;
    C char_cnt[512]{};
    std::vector<CHAR> char_set;
    BitSet<uint8_t> dlr_bits;
    using encoded_bits = BitSet<uint8_t>;
    encoded_bits bit[512];


    struct HuffmanNode {
        HuffmanNode *lson, *rson;
        CHAR c;

        HuffmanNode(const CHAR &c, HuffmanNode *lson, HuffmanNode *rson) : lson(lson), rson(rson), c(c) {};
    } *root = nullptr;

    using P = std::pair<C, HuffmanNode *>;

    explicit Huffman(uint32_t size) : Buffer(size) {}

    struct myGreater : std::binary_function<P, P, bool> {
        bool operator()(const P &x, const P &y) const { return x.first > y.first; }
    };

    void clear_and_do() override {
        if (buffer_offset == 0) {//保证不会对空集编码
            return;
        }
        totalBit = calc();
        //print_code();
        /*编码结果的bit数n*/
        for (int j = 24; j >= 0; j -= 8) {
            CHAR tmp = totalBit >> j;
            f(tmp);
        }
        /*1byte 字符集大小m */
        f(uint8_t(totalChar - 1));
        /*2byte,k= dlr_bit.Size*/
        f(dlr_bits.Size >> 8);
        f(dlr_bits.Size & 0xff);

        for (auto it: dlr_bits.bits) {
            f(it);
        }
        for (auto it: char_set) {
            f(it);
        }
        encoded_bits res;
        //res.resize(BUFFER_SIZE);
        for (int i = 0; i < buffer_offset; ++i) {
            const auto &code = bit[buffer[i]];//&！！！！
            res.append(code);
        }
        for (auto it: res.bits) {
            f(it);
        }
        // Log(std::cout,totalBit,res.Size);
        memset(char_cnt, 0, sizeof(char_cnt));
        buffer_offset = 0;
        char_set.clear();
        dlr_bits.clear();
    }

    void dfs(HuffmanNode *cur_node, encoded_bits cur_bits) {
        if (cur_node->lson || cur_node->rson) {
            dlr_bits.push_back(false);//不是叶子节点
            auto new_bits = (cur_bits);
            new_bits.push_back(false);
            if (cur_node->lson) {
                dfs(cur_node->lson, new_bits);
            }
            new_bits.pop_back();
            new_bits.push_back(true);
            if (cur_node->rson) {
                dfs(cur_node->rson, new_bits);
            }
            new_bits.pop_back();
        } else {
            dlr_bits.push_back(true);//是叶子节点
            char_set.push_back((cur_node->c));
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
        totalChar = 0;
        for (int i = 0; i < 512; ++i) {
            if (char_cnt[i] > 0) {
                ++totalChar;
                Huffman_heap.push(P{char_cnt[i], new HuffmanNode(i, nullptr, nullptr)});
            }
        }
        C res = 0;
        root = new HuffmanNode(0, Huffman_heap.top().second, nullptr);
        for (; Huffman_heap.size() > 1;) {
            auto top1 = Huffman_heap.top();
            Huffman_heap.pop();
            auto top2 = Huffman_heap.top();
            Huffman_heap.pop();
            auto new_top = P{top1.first + top2.first, root = new HuffmanNode(0, top1.second, top2.second)};
            res += top1.first + top2.first;
            Huffman_heap.push(new_top);
        }
        auto tmp = encoded_bits();
        dfs(root, tmp);
        return totalChar == 1 ? Huffman_heap.top().first : res;
    }

    void print_code() {
        for (int i = 0; i < 512; ++i) {
            if (bit[i].Size > 0) {
                std::cout << i << '\t';
                bit[i].out();
                std::cout << '\n';
            }
        }
    }
};

struct HuffmanDecode : public Huffman {
    C dlr_bits_size = 0;

    explicit HuffmanDecode(uint32_t size) : Huffman(size) {}

    HuffmanNode *undfs(int &pt, int &char_num) {
        if (dlr_bits.Test(pt)) {
            ++pt;
            return new HuffmanNode(char_set[char_num++], nullptr, nullptr);
        }
        return new HuffmanNode(0, undfs(++pt, char_num), undfs(pt, char_num));
    }

    void clear_and_do() override {
        //totalBit = calc();
        dlr_bits.clear();
        char_set.clear();
        for (int i = 4 + 1 + 2; i < 4 + 1 + 2 + (dlr_bits_size + 7) / 8; ++i) {
            dlr_bits.push_back(buffer[i]);
        }
        dlr_bits.Size = dlr_bits_size;
        for (int i = 4 + 1 + 2 + (dlr_bits_size + 7) / 8; i < 4 + 1 + 2 + (dlr_bits_size + 7) / 8 + totalChar; ++i) {
            char_set.push_back(buffer[i]);
        }


        int pt = 0;
        int char_num = 0;
        if (char_set.size() == 1) {
            root = new HuffmanNode(0, new HuffmanNode(char_set[0], 0, 0), 0);
        } else {
            root = undfs(pt, char_num);
            dfs(root, encoded_bits());
        }

        encoded_bits code;
        for (int i = 4 + 1 + 2 + totalChar + (dlr_bits_size + 7) / 8;
             i < 4 + 1 + 2 + totalChar + (dlr_bits_size + 7) / 8 + (totalBit + 7) / 8; ++i) {
            code.push_back(buffer[i]);
        }
        code.Size = totalBit;

        auto cur_node = root;
        code.forEach([&cur_node, this](bool x) {
            if (x) {
                cur_node = cur_node->rson;
            } else {
                cur_node = cur_node->lson;
            }
            if (__builtin_expect(cur_node->lson == nullptr && cur_node->rson == nullptr, 0)) {
                this->f(cur_node->c);
                cur_node = this->root;
            }
        });
        buffer_offset = 0;
        dlr_bits_size = 0;
        totalChar = 0;
        totalBit = 0;
    }

    void push(const CHAR &x) override {
        buffer[buffer_offset++] = x;
        if (buffer_offset == 7) {
            for (int i = 0; i < 4; ++i) {
                totalBit <<= 8;
                totalBit |= buffer[i];
            }
            totalChar = C(buffer[4]) + 1;
            dlr_bits_size = (C(buffer[5]) << 8) | buffer[6];
        } else if (buffer_offset == 4 + 1 + 2 + (dlr_bits_size + 7) / 8 + totalChar + (totalBit + 7) / 8) {
            clear_and_do();
        }
    }
};

#endif //BUPT_FINAL_HUFFMAN_H
