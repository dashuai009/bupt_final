//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_AC_H
#define BUPT_FINAL_AC_H

#include<queue>
#include<vector>
#include "Log.h"
using STR = std::string;
std::vector<STR> pattern_str;

struct TrieNode {
    TrieNode *nxt[128];
    TrieNode *fail;
    int depth;
    int end; //第几个模式串，之后要放到占位符中间，0表示当前不是匹配串的结尾字符
    char c;

    TrieNode(char c, int depth) : depth(depth), end(0), c(c) {
        fail=nullptr;
        for (auto &it: nxt) {
            it = nullptr;
        }
    }
};


/**
 * @brief 向trie_tree中插入一个模式串，空串被自动忽略
 *
 * @param str 模式串
 * @param root tried_tree的根节点
 */
void trie_insert(const std::string &str, TrieNode *&root) {
    TrieNode *cur_node = root;
    for (char it: str) {
        if (cur_node->nxt[(int) it] == NULL) {
            cur_node->nxt[(int) it] = new TrieNode(it, cur_node->depth + 1);
        }
        cur_node = cur_node->nxt[(int) it];
    }
    if (str.length() > 0 && cur_node->end == 0) {
        //忽略空串，忽略已经加进去的模式串
        pattern_str.push_back(str);
        cur_node->end = pattern_str.size();
    }
}

TrieNode *build_trie(char config_file[]) {
    FILE *config = fopen(config_file, "r");
    std::string preset_str = "";
    char c;
    TrieNode *root = new TrieNode((char) 0, 0);
    if (config) {
        while ((c = getc(config)) != EOF) {
            if (c == '\n') {
                trie_insert(preset_str, root);
                preset_str = "";
            } else {
                preset_str += c;
            }
        }
        trie_insert(preset_str, root);
        fclose(config);
    }
    Log(std::cout, "共有：" , pattern_str.size() , "个模式串");
    return root;
}

void build_AC(TrieNode *root) {
    std::queue<TrieNode *> Q;
    for (auto &root_son: root->nxt) {
        if (root_son != NULL) {
            root_son->fail = root;
            Q.push(root_son);
        }
    }

    while (!Q.empty()) {
        TrieNode *frt = Q.front();
        Q.pop();
        int i = 0;
        for (auto &son: frt->nxt) {
            //必须要用引用 son = &frt->nxt[i]
            if (son != NULL) {
                son->fail = frt->fail->nxt[i] ? frt->fail->nxt[i] : root;
                Q.push(son);
            } else {
                son = frt->fail->nxt[i];
            }
            ++i;
        }
    }
}

#endif //BUPT_FINAL_AC_H

