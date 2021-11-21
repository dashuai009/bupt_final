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


const char help_text[] =
        "只可接受可见字符(ascii码为32~127)和'\\n'"
        "字符的纯文本文件。可接受如下参数列表：\n"
        "-h\t\t\t可选，显示此帮助信息\n"
        "-i <input.file>\t\tt必选，输入文件\n"
        "-o <output.file>\t可选，默认覆盖原文件\n"
        "-p <config.txt>\t\t可选，配置文件，默认提供defalutConfig.txt文件\n"
        "-d\t\t\t解压模式\n"
        "-q\t\t\t可选，安静模式\n";


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
    out_buf.clear_and_do = [output_ptr, &out_buf]() {
        fwrite(out_buf.buffer, sizeof(T), out_buf.buffer_offset, output_ptr);
        out_buf.buffer_offset = 0;
    };
    return output_ptr;
}

Buffer<char> out_buf;
Huffman H;

Buffer2<char> AC_buffer;
Buffer2<INFO> AC_info_stack;
Buffer<char> input_buffer;

void zip(TrieNode *root, char input_file_name[], char output_file_name[]) {
    FILE *output_ptr = writeOutFile(output_file_name, out_buf);
    H.f = [](const char &c) {
        out_buf.push(c);
    };
    AC_buffer.f = [](const char &c) {
        H.push(c);
    };
    input_buffer.f = [root](const char &c) {
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

char input_file[100] = "test_in.txt";
char output_file[100];
char config_file[100] = "defaultConfig.txt";
int mode = 0;

int main(int argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "h::i:o:p:d::")) != -1) {
        switch (opt) {
            case 'h':
                printf("%s", help_text);
                exit(0);
                break;
            case 'i':
                strcpy(input_file, optarg);
                if (strlen(output_file) == 0) {
                    strcpy(output_file, input_file);
                }
                break;
            case 'o':
                strcpy(output_file, optarg);
                output_file[strlen(optarg)] = 0;
                break;
            case 'p':
                strcpy(config_file, optarg);
                output_file[strlen(optarg)] = 0;
                break;
            case 'q':
                quiet = true;
                break;
            case 'd':
                mode = 1; //解压模式
                break;
            default:
                break;
        }
    }
    Log(std::cout, "input_file:\t\t", input_file);
    Log(std::cout, "output_file:\t", output_file);
    Log(std::cout, "config_file:\t", config_file);
    TrieNode *root = build_trie(config_file);
    if (mode == 0) {
        build_AC(root);
        zip(root, input_file, output_file);
    } else {
        unzip(input_file, config_file, output_file);
    }

    return 0;
}
