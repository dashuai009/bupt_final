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
#include "INFO.h"

namespace AC {
#define TrieNodePtr int

struct TrieNode {
    TrieNodePtr nxt[128]{};
    TrieNodePtr fail;
    int depth;
    int end; //第几个模式串，之后要放到占位符中间，0表示当前不是匹配串的结尾字符
    char c;

    TrieNode():fail(0),depth(0),end(0),c(0){

    }

    TrieNode(char c, int depth) : depth(depth), end(0), c(c) {
        fail = 0;
        for (auto &it: nxt) {
            it = 0;
        }
    }
};

template<int N>
class AutoMaton {
    InfoStack &infoStack;
    //std::array<TrieNode, N> node;
    TrieNode node[N];
    const TrieNodePtr root = 1;
    int nodeCnt = 1;
public:
    void match(const char &c) {
        static long long pt = 0;
        static TrieNodePtr cur = root;//root == 1
        cur = node[cur].nxt[uint8_t(c)] !=0 ? node[cur].nxt[uint8_t(c)] : root;
        static int cnt = 0;
        for (auto it = cur; it != 0; it = node[it].fail) {
            if (node[it].end) {
                infoStack.push_st(INFO{pt, 0, node[it].end - 1});
            }
        }
        ++pt;
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
        TrieNodePtr cur = root;
        for (auto it: str) {
            assert((int) it < 128);
            if (node[cur].nxt[(unsigned char) it] == 0) {
                node[++nodeCnt] = TrieNode(it,node[cur].depth + 1);
                node[cur].nxt[(int) it] = nodeCnt;
            }
            cur = node[cur].nxt[(int) it];
        }
        if (node[cur].end == 0) {
            //如果str已经加进去了，cur_node的编号不为0
            node[cur].end = No;
        }
    }

    void build_AC() {
        std::queue<TrieNodePtr> Q;
        for (auto &root_son: node[root].nxt) {
            if (root_son != 0) {
                node[root_son].fail = root;
                Q.push(root_son);
            }
        }

        while (!Q.empty()) {
            auto frt = Q.front();
            Q.pop();
            int i = 0;
            for (auto &son: node[frt].nxt) {
                //必须要用引用 son = &frt->nxt[i]
                if (son != 0) {
                    node[son].fail = node[node[frt].fail].nxt[i] ? node[node[frt].fail].nxt[i] : root;
                    Q.push(son);
                } else {
                    son = node[node[frt].fail].nxt[i];
                    //assert(son != frt);
                }
                ++i;
            }
        }
    }

     AutoMaton(InfoStack &infoSatck) : infoStack(infoSatck) {}

    void build_from_pattern() {
        int i = 0;
        for (auto &s: PatternStr::pattern_str) {
            trie_insert(s, ++i);
        }
        build_AC();
        std::cout << nodeCnt << '\n';
    }
};

}//end_namespace_AC

#endif //BUPT_FINAL_AC_H