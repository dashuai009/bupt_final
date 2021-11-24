#include <bits/stdc++.h>
#include <unistd.h>

#ifndef BUPT_FINAL_AC_H

#include "AC.h"

#endif
#ifndef BUPT_FINAL_BUFFFER_H

#include "Buffer.h"

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


using INFO = std::pair<long long, TrieNode *>;
auto start_pos = [](const INFO &x) {
    return x.first - x.second->depth + 1;
};
auto longer = [](const INFO &x, const INFO &y) {
    return x.second->depth > y.second->depth;
};

void push_st(long long end_pt, TrieNode *end_node, Buffer2<INFO> &stack, Buffer2<char> &out_buf) {
    auto end_info = std::make_pair(end_pt, end_node);


    int cnt = 0, cnt_pop = 0;
    while ((stack.size() > cnt_pop) && start_pos(end_info) <= start_pos(stack.get(-cnt_pop - 1)) &&
           longer(end_info, stack.get(-cnt_pop - 1))) {
        cnt += stack.get(-cnt_pop - 1).second->depth - 1;
        ++cnt_pop;
    }
    if (stack.size() <= cnt_pop || start_pos(end_info) > stack.get(-cnt_pop - 1).first) {
        stack.pop_back(cnt_pop);
        out_buf.pop_back(end_node->depth - cnt);
        out_buf.push(char(end_node->end + int('A') - 1));
        stack.push(end_info);
    }
}

void AC_match(const char &c, TrieNode *root, const std::function<void(long long, TrieNode *)> &push_st) {
    static long long pt = 0;
    static TrieNode *cur_node = root;
    cur_node = cur_node->nxt[int(c)] ? cur_node->nxt[int(c)] : root;
    for (auto it = cur_node; it != NULL; it = it->fail) {
        if (it->end) {
            push_st(pt, it);
        }
    }
    ++pt;
}

/**
 * read file and clear input_buf
 * @param input_file_name
 * @param input_buf
 */
void readInPutFile(char input_file_name[], Buffer<char> &input_buf) {
    FILE *input_ptr = fopen(input_file_name, "rb");
    if (!input_ptr) {
        std::cerr << "input_file error!\n";
        return;
    }
    // obtain file size:
    fseek(input_ptr, 0, SEEK_END);
    auto input_size = ftell(input_ptr);
    rewind(input_ptr);

    for (uint64_t i = 0; i < input_size / input_buf.BUFFER_SIZE; ++i) {
        fread(input_buf.buffer, 1, input_buf.BUFFER_SIZE, input_ptr);
        input_buf.buffer_offset = input_buf.BUFFER_SIZE;
        input_buf.clear_and_do();
        //Log(std::cout, i);
    }
    if (input_size % input_buf.BUFFER_SIZE > 0) {
        fread(input_buf.buffer, 1, input_size % input_buf.BUFFER_SIZE, input_ptr);
        input_buf.buffer_offset = input_size % input_buf.BUFFER_SIZE;
        input_buf.clear_and_do();
    }
    fclose(input_ptr);
}

template<typename T>
FILE *writeOutFile(char output_file_name[], Buffer<T> &out_buf) {
    FILE *output_ptr = fopen(output_file_name, "wb");
    out_buf.f = [output_ptr](const T &x) {
        fputc(uint8_t(x), output_ptr);
    };
    //out_buf.clear_and_do = [output_ptr, &out_buf]() {
    //    fwrite(out_buf.buffer, sizeof(T), out_buf.buffer_offset, output_ptr);
    //    out_buf.buffer_offset = 0;
    //};
    return output_ptr;
}

TrieNode *root = nullptr;
Buffer<char> out_buf;
Huffman H;

Buffer2<char> AC_buffer;
Buffer2<INFO> AC_info_stack;
Buffer<char> input_buffer;

void zip(char input_file_name[], char output_file_name[]) {
    FILE *output_ptr = writeOutFile(output_file_name, out_buf);
    H.f = [](const char &c) {
        out_buf.push(c);
    };
    AC_buffer.f = [](const char &c) {
        H.push(c);
    };
    input_buffer.f = [](const char &c) {
        AC_buffer.push(c);
        AC_match(c, root, [](long long end_pt, TrieNode *end_node) {
            push_st(end_pt, end_node, AC_info_stack, AC_buffer);
        });
    };
    readInPutFile(input_file_name, input_buffer);
    AC_buffer.clear_and_do();
    H.clear_and_do();


    out_buf.clear_and_do();
    fclose(output_ptr);
}

void unzip(char target_file[], char config_file[], char output_file[]) {
    FILE *target = fopen(target_file, "r");
    FILE *output = fopen(output_file, "w");
    auto out_putc = [&output](const char &c) {
        fputc(c, output);
    };
    Buffer<char> out_stream(out_putc);
    HuffmanDecode H;
    H.f = [&out_stream, &output](const char &c) {
        if (c >= 'A') {
            for (auto &x: pattern_str[c - 'A']) {
                out_stream.push(x);
            }
        } else {
            out_stream.push(c);
        }
        //out_stream.push(c);
        //fputc(c, output);
    };
    //for (char c = getc(target); c != EOF; c = getc(target)) {
    //    H.push(c);
    //}
    int c;
    while ((c = getc(target)) != EOF) {
        H.push(c);
    }
    //H.clear_and_do();
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
