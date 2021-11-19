#include <bits/stdc++.h>
#include <unistd.h>
#include "Buffer.h"
#include "Log.h"
#include "AC.h"

const char help_text[] =
        "只可接受可见字符(ascii码为32~127)和'\\n'"
        "字符的纯文本文件。可接受如下参数列表：\n"
        "-h\t\t\t可选，显示此帮助信息\n"
        "-i <input.file>\t\tt必选，输入文件\n"
        "-o <output.file>\t可选，默认覆盖原文件\n"
        "-p <config.txt>\t\t可选，配置文件，默认提供defalutConfig.txt文件\n"
        "-d\t\t\t解压模式\n"
        "-q\t\t\t可选，安静模式\n";


using STACK = std::vector<std::pair<long long, TrieNode *>>;


Buffer2<char> out_buf;
using INFO = std::pair<long long, TrieNode *>;
Buffer2<INFO> stack;
auto start_pos = [](const INFO &x) {
    return x.first - x.second->depth + 1;
};
auto longer = [](const INFO &x, const INFO &y) {
    return x.second->depth > y.second->depth;
};
void push_st(long long end_pt, TrieNode *end_node) {
    auto end_info = std::make_pair(end_pt, end_node);


    int cnt = 0, cnt_pop = 0;
    while ((stack.size() > cnt_pop) && start_pos(end_info) <= start_pos(stack.get(-cnt_pop - 1)) &&
           longer(end_info, stack.get(-cnt_pop - 1))) {
        cnt += stack.get(-cnt_pop - 1).second->depth - 1;
        ++cnt_pop;
    }
    if (stack.size() <= cnt_pop || end_pt - end_node->depth + 1 > stack.get(-cnt_pop - 1).first) {
        stack.pop_back(cnt_pop);
        out_buf.pop_back(end_node->depth - cnt);
        out_buf.push(char(end_node->end + int('A') - 1));
        stack.push(end_info);
    }
}

void AC_match(const char &c,TrieNode *root) {
    static long long pt = 0;
    static TrieNode *cur_node = root;
    //std::cerr << c << '\n';
    out_buf.push(c);
    cur_node = cur_node->nxt[int(c)] ? cur_node->nxt[int(c)] : root;
    for (auto it = cur_node; it != NULL; it = it->fail) {
        if (it->end) {
            push_st(pt, it);
        }
    }
    ++pt;
    //out_s += c;
}

inline void match_digit(char &c, TrieNode *root) {
    //auto f = std::bind(&AC_match, std::placeholders::_1, root);
    static auto f = [root](auto &&PH1) { return AC_match(std::forward<decltype(PH1)>(PH1), root); };
    static Buffer<char> B(f);
    B.push(c);
}

void zip(TrieNode *root, char input_file[], char output_file[]) {
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "w");
    out_buf.f = [&output](const char &c) {
        putc(c, output);
    };
    if (!input) {
        std::cerr << "input_file error!\n";
        return;
    }
    char c;
    auto ff = [&root](char &c) {
        AC_match(c, root);
    };
    auto B = Buffer<char>(ff);
    while ((c = getc(input)) != EOF) {
        B.push(c);
    }
    B.clear_and_do();
    fclose(input);
    out_buf.clear_and_do();
    fclose(output);
    //print_out(out_s, st, output_file);
}

void unzip(char target_file[], char config_file[], char output_file[]) {
    FILE *target = fopen(target_file, "r");
    FILE *output = fopen(output_file, "w");
    auto out_putc = [&output](const char &c) {
        fputc(c, output);
    };
    Buffer<char> out_stream(out_putc);
    for (char c = getc(target); c != EOF; c = getc(target)) {
        if (c >= 'A') {
            for (auto &x: pattern_str[c - 'A']) {
                out_stream.push(x);
            }
        } else {
            out_stream.push(c);
        }
    }
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
