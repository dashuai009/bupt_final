#include <bits/stdc++.h>
#include <zlib.h>

#ifndef BUPT_FINAL_AC_H

#include "AC.h"

#endif
#ifndef BUPT_FINAL_BUFFFER_H

#include "Buffer.h"

#endif

#ifndef BUPT_FINAL_CIRCULARQUEUE_H

#include "CircularQueue.h"

#endif
#ifndef BUPT_FINAL_GUFFMAN_H

#include "Huffman.h"

#endif
#ifndef BUPT_FINAL_LOG_H
#include "Log.h"
#endif
#ifndef BUPT_FINAL_SETOPTIONS_H

#include "SetOptions.h"

#endif

#ifndef  BUPT_FINAL_READANDWRITEFILE_H

#include "ReadAndWriteFile.h"

#endif //BUPT_FINAL_READANDWRITEFILE_H


template<typename T>
void applyF(const T x, const std::function<void(uint8_t)> &f) {
    for (int i = (sizeof(T) - 1) * 8; i >= 0; i -= 8) {
        f(x >> i);
    }
}

struct INFO {
    long long end_pos;
    int forward;
    int len;

    [[nodiscard]] inline uint64_t length() const {
        return forward == 0 ? pattern_str[len].length() : len;
    }

    [[nodiscard]] inline long long start_pos() const {
        return end_pos + 1 - length();
    }
};

auto info_include = [](const INFO &x, const INFO &y) {//已经满足x.pt>=y.pt，满足”起始位置更靠前“则 x 包括了 y。
    return x.start_pos() <= y.start_pos();
};

void push_st(const INFO &new_info, CircularQueue<INFO> &stack) {
    int cnt_pop = 0;
    while ((stack.size() > cnt_pop) && info_include(new_info, stack.get(-cnt_pop - 1))) {
        ++cnt_pop;
    }
    if (stack.size() == cnt_pop || new_info.start_pos() > stack.get(-cnt_pop - 1).end_pos) {//栈空，或不冲突
        stack.pop_back(cnt_pop);
        stack.push(new_info);
    } else if (stack.size() > cnt_pop && new_info.length() > stack.get(-cnt_pop - 1).length() &&
               new_info.start_pos() > stack.get(-cnt_pop - 1).start_pos()) {//栈不空，冲突，若更好的
        stack.pop_back(cnt_pop + 1);
        stack.push(new_info);
    }
}

void AC_match(const char &c, TrieNode *root, CircularQueue<INFO> &stack) {
    static long long pt = 0;
    static TrieNode *cur_node = root;
    cur_node = cur_node->nxt[uint8_t(c)] ? cur_node->nxt[uint8_t(c)] : root;
    bool flag = false;
    for (auto it = cur_node; it != nullptr; it = it->fail) {
        if (it->end) {
            push_st(INFO{pt, 0, it->end - 1}, stack);
            flag = true;
        }
    }
    if (!flag) {

    }
    ++pt;
}


constexpr uint32_t N = 64 * 1024;
OutBuffer out_buf(N);
TrieNode *root = nullptr;
Huffman H = Huffman(N);
CircularQueue<char> AC_buffer(N * 2 + 1);
CircularQueue<INFO> AC_info_stack(N * 10 + 1);
Buffer<uint8_t> input_buffer(N);


/*
 * 4 byte 0/1控制串的bit长度 n
 * 4bytes 待编码区域的byte长度 k。
 * (n+7)/8 比特的 0/1控制串
 * k bytes的 待编码字符。
 * */
struct MatchBuffer {
    uint32_t BUFFER_SIZE = 0;
    uint32_t buffer_offset = 0;
    std::function<void(uint8_t)> f = nullptr;
    BitSet<uint8_t> ctrl_flow;
    uint8_t *text_buf = nullptr;
    uint8_t *text_out = nullptr;
    uint8_t *text_test = nullptr;

    explicit MatchBuffer(uint32_t buffer_size) : BUFFER_SIZE(buffer_size) {
        text_buf = new uint8_t[buffer_size];
        text_out = new uint8_t[buffer_size];
        text_test = new uint8_t[buffer_size];
    }

    ~MatchBuffer() {
        delete[] text_buf;
        delete[] text_out;
        delete[]text_test;
    }


    void clear_and_do() {
        //Log(std::cout, ctrl_flow.Size);
        //Log(std::cout, need_to_huffman.size());
        uint32_t zlib_size = 0;
        compress(text_out, reinterpret_cast<uLongf *>(&zlib_size), text_buf, buffer_offset);
        uint32_t  fff = 0;
        uncompress(text_test, reinterpret_cast<uLongf *>(&fff), text_out, zlib_size);
        Log(std::cout,"fff",fff);
        Log(std::cout,"origin",buffer_offset);
        applyF(ctrl_flow.Size, f);//4 bytes ==buffer_offset
        applyF(zlib_size, f);//4 bytes
        for (auto it: ctrl_flow.bits) {
            f(it);
        }
        for (int i = 0; i < zlib_size; ++i) {
            f(text_out[i]);
        }
        Log(std::cout, "crtl_flow_size:", ctrl_flow.Size, "bits", (ctrl_flow.Size + 7) / 8, "bytes");
        Log(std::cout, "zlib size:", zlib_size);
        ctrl_flow.clear();
        buffer_offset = 0;
    }

    void push(const bool &ctrl_bit, const uint8_t &c) {
        ctrl_flow.push_back(ctrl_bit);
        text_buf[buffer_offset++] = c;
        if (ctrl_flow.Size == BUFFER_SIZE) {
            clear_and_do();
        }
    }

} after_match_buffer(N);


struct UnzipMatchBuffer : Buffer<uint8_t> {
    explicit UnzipMatchBuffer(uint32_t buffer_size) : Buffer<uint8_t>(buffer_size) {}

    uint32_t ctrl_flow_size = 0;
    uint32_t info_size = 0;
    BitSet<uint8_t> ctrl_flow;


    void clear_and_do() override {
        for (int i = 8; i < 8 + (ctrl_flow_size + 7) / 8; ++i) {
            ctrl_flow.push_back(buffer[i]);
        }
        ctrl_flow.Size = ctrl_flow_size;
        Log(std::cout, "crtl_flow_size:", ctrl_flow.Size, "bits", (ctrl_flow.Size + 7) / 8, "bytes");
        Log(std::cout, "待编码字符流：", info_size, "bytes");
        static int pt = 0;
        pt = 8 + (ctrl_flow_size + 7) / 8;
        ctrl_flow.forEach([this](const bool x) {
            if (x) {
                this->f(this->buffer[pt]);
                ++pt;
            } else {
                uint32_t a = (uint32_t(this->buffer[pt]) << 8) | (this->buffer[pt + 1]);
                uint32_t b = this->buffer[pt + 2];
                if (a == 0) {
                    for (const auto &it: pattern_str[b]) {
                        this->f(it);
                    }
                } else {
                    Log(std::cout, "\n", (ctrl_flow_size + 7) / 8, pt, "error");
                    exit(0);
                }
                pt += 3;
            }
        });
        pt = 0;
        ctrl_flow_size = 0;
        info_size = 0;
        ctrl_flow.clear();
        buffer_offset = 0;
    }

    void push(const uint8_t &c) override {
        buffer[buffer_offset++] = c;
        if (buffer_offset <= 4) {
            ctrl_flow_size = (ctrl_flow_size << 8) | c;
        } else if (buffer_offset <= 8) {
            info_size = (info_size << 8) | c;
        } else if (buffer_offset == 8 + (ctrl_flow_size + 7) / 8 + info_size) {
            clear_and_do();
        }
    }
};

UnzipMatchBuffer UM(N * 50);

void zip(char input_file_name[], char output_file_name[]) {
    writeOutFile(output_file_name, out_buf);
    H.f = [](const uint8_t &c) {
        //printf("%x ", c);
        out_buf.push(c);
    };
    after_match_buffer.f = [](const uint8_t &c) {
        //printf("%x ", c);
        //H.push(c);
        out_buf.push(c);
    };
    AC_buffer.f = [](const char &c) {
        static long long pt = 0;
        if (AC_info_stack.size() > 0) {
            //Log(std::cout, pt, c);
            //Log(std::cout, pattern_str[10]);
            auto info_head = AC_info_stack.get(0);
            if (info_head.start_pos() == pt) {
                after_match_buffer.push(false, info_head.len);
            } else if (info_head.start_pos() < pt && pt <= info_head.end_pos) {

            } else {
                after_match_buffer.push(true, c);
                //printf("%c", c);
            }
            if (info_head.end_pos == pt) {
                AC_info_stack.start_pos++;
            }
        } else {
            after_match_buffer.push(true, c);
        }
        ++pt;


    };
    input_buffer.f = [](const char &c) {
        AC_match(c, root, AC_info_stack);
        AC_buffer.push(c);
        //printf("%x ", c);
    };
    readInPutFile(input_file_name, input_buffer);
    AC_buffer.clear_and_do();
    after_match_buffer.clear_and_do();
    H.clear_and_do();
    out_buf.clear_and_do();
    fclose(out_buf.output_ptr);
}

Buffer<uint8_t> out_stream(N);
HuffmanDecode HD = HuffmanDecode(N * 50);

void unzip(char target_file[], char config_file[], char output_file[]) {
    FILE *target = fopen(target_file, "r");
    FILE *output = fopen(output_file, "w");

    UM.f = [](const uint8_t &c) {
        out_stream.push(c);
    };
    out_stream.f = [&output](const char &c) {
        // printf("%c", c);
        fputc(c, output);
    };
    HD.f = [](const uint8_t &c) {// 这地方写的有点hack
        UM.push(c);
        // printf("%x ", c);
    };
    int c;
    while ((c = getc(target)) != EOF) {
        //HD.push(c);
        UM.push(c);
    }
    //HD.clear_and_do();
    out_stream.clear_and_do();
    fclose(target);
    fclose(output);
}


int main(int argc, char *argv[]) {
    set_options(argc, argv);
    root = build_trie(config_file);
    if (mode == 0) {
        build_AC(root);
        zip(input_file, output_file);
    } else {
        unzip(input_file, config_file, output_file);
    }

    return 0;
}
