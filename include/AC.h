/**
 * ACAutomaton
 * AC 自动机是 以 Trie 的结构为基础，结合 KMP 的思想 建立的。
 *
 * 简单来说，建立一个 AC 自动机有两个步骤：
 *
 * - 基础的 Trie 结构：将所有的模式串构成一棵 Trie。
 * - KMP 的思想：对 Trie 树上所有的结点构造失配指针。
 * 然后就可以利用它进行多模式匹配了。
 */

//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_AC_H
#define BUPT_FINAL_AC_H

#include<queue>
#include<vector>
#include "Log.h"

namespace AC {
using STR = std::string;
std::vector<STR> pattern_str;

struct TrieNode {
    TrieNode *nxt[128]{};
    TrieNode *fail;
    int depth;
    int end; //第几个模式串，之后要放到占位符中间，0表示当前不是匹配串的结尾字符
    char c;

    TrieNode(char c, int depth) : depth(depth), end(0), c(c) {
        fail = nullptr;
        for (auto &it: nxt) {
            it = nullptr;
        }
    }
};


class AutoMaton {
public:
    TrieNode *root = new TrieNode((char) 0, 0);

    std::vector<TrieNode*> nodeSet;
    void deleteNode(TrieNode *cur) {
        for (auto it: cur->nxt) {
            if (it != nullptr && it != cur) {
                deleteNode(it);
                //it = nullptr;//这个无所谓
            }
        }
        delete cur;
    }

    /**
     * @brief 向trie_tree中插入一个模式串，空串被自动忽略
     *
     * @param str 模式串
     * @param root tried_tree的根节点
     */
    void trie_insert(const std::string &str, int No) {
        if (str.length() == 0) {//忽略空串
            return;
        }
        TrieNode *cur_node = root;
        for (auto it: str) {
            assert((int)it < 128);
            if (cur_node->nxt[(unsigned char) it] == nullptr) {
                cur_node->nxt[(int) it] = new TrieNode(it, cur_node->depth + 1);
                nodeSet.push_back(cur_node->nxt[(int)it]);
            }
            assert(cur_node != cur_node -> nxt[(int)it]);
            cur_node = cur_node->nxt[(int) it];
        }
        if (cur_node->end == 0) {
            //如果str已经加进去了，cur_node的编号不为0
            cur_node->end = No;
        }
    }

    void build_AC() {
        std::queue<TrieNode *> Q;
        for (auto &root_son: root->nxt) {
            if (root_son != nullptr) {
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
                if (son != nullptr) {
                    son->fail = frt->fail->nxt[i] ? frt->fail->nxt[i] : root;
                    Q.push(son);
                } else {
                    son = frt->fail->nxt[i];
                    //assert(son != frt);
                }
                ++i;
            }
        }
    }

    AutoMaton() = default;
    void build_from_pattern() {
        int i = 0;
        for (auto &s: pattern_str) {
            trie_insert(s, ++i);
        }
        build_AC();
    }

    ~AutoMaton() {
        delete root;
        for(auto it:nodeSet){
            delete it;
        }
    }
};

void readPatternStr(char config_file[]) {
    FILE *config = fopen(config_file, "r");
    assert(config != nullptr);
    std::string s;
    char c;
    while ((c = getc(config)) != EOF) {
        if (c == '\n') {
            if (s.length() > 0) {
                pattern_str.push_back(s);
            }
            s = "";
        } else {
            s += c;
        }
    }
    fclose(config);
}

}//end_namespace_AC

#endif //BUPT_FINAL_AC_H