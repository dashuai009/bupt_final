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
#include<cstring>
#include<cassert>

namespace AC {
namespace TrieNodeArray {
using TrieNodePtr = int;

struct TrieNode {
    TrieNodePtr nxt[128]{};
    TrieNodePtr fail;
    TrieNodePtr nodeEnd;
    TrieNodePtr last;
    int depth;
    char c;

    TrieNode() : fail(0), nodeEnd(0), last(0), depth(0), c(0) {
        for (auto &it: nxt) {
            it = 0;
        }

    }

    TrieNode(char c, int depth) : fail(0), nodeEnd(0), last(0), depth(depth), c(c) {
        for (auto &it: nxt) {
            it = 0;
        }
    }
};

template<int nodeN>
class AutoMaton {
    InfoStack &infoStack;

    /// ======= trie ========
    std::array<TrieNode, nodeN> node;
//    std::array<TrieNodePtr, N> fail,nodeEnd,last;
    const TrieNodePtr root = 0;
    //TrieNodePtr fail[N], nodeEnd[N], last[N];
    //static constexpr TrieNodePtr root = 1;
    int nodeCnt = 1;



    /// ==== private method =====
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
                node[nodeCnt] = TrieNode(it, node[cur].depth + 1);
                node[cur].nxt[(int) it] = nodeCnt;
                ++nodeCnt;
                assert(nodeCnt <= nodeN);
            }
            cur = node[cur].nxt[(int) it];
        }
        if (node[cur].nodeEnd == 0) {
            //如果str已经加进去了，cur_node的编号不为0
            node[cur].nodeEnd = No;
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
                    node[son].fail = node[node[frt].fail].nxt[i];
                    Q.push(son);
                } else {
                    son = node[node[frt].fail].nxt[i];
                    //assert(son != frt);
                }
                ++i;
            }
            node[frt].last = node[node[frt].fail].nodeEnd ? node[frt].fail
                                                          : node[node[frt].fail].last;
        }
    }

    /// ==== match variable =====
    int endCnt = 0;
    int visitedCnt = 0;
public:
    explicit AutoMaton(InfoStack &infoSatck) : infoStack(infoSatck) {}

    void match(const char &c) {
        static long long pt = 0;
        static TrieNodePtr matchCur = root;//root == 0
        matchCur = node[matchCur].nxt[uint8_t(c)];
        for (auto it = matchCur; it; it = node[it].fail) {
            //++visitedCnt;
            if (node[it].nodeEnd) {
                //if (nodeEnd[it]) {
                infoStack.push_st(INFO{pt, 0, node[it].nodeEnd - 1});
                //++endCnt;
            }
        }
        ++pt;
    }


    void init(const std::vector<std::string> &strs) {
        node.fill(TrieNode());
        nodeCnt = 1;
        endCnt = 0;
        visitedCnt = 0;
        int i = 0;
        for (auto &s: strs) {
            trie_insert(s, ++i);
        }
        build_AC();
//        std::cout << "The size of trie is " << nodeCnt << '\n';
    }
};

}//end_namespace_TrieNodeArray

namespace TrieNodeHeap {


struct TrieNode {
    using TrieNodePtr = TrieNode *;
    TrieNodePtr nxt[128]{};
    TrieNodePtr fail;
    int depth;
    int end;
    TrieNodePtr last;
    char c;

    TrieNode() : fail(nullptr), depth(0), end(0), last(nullptr), c(0) {
        for (auto &it: nxt) {
            it = nullptr;
        }
    }

    TrieNode(char c, int depth) : fail(nullptr), depth(depth), end(0), last(nullptr), c(c) {
        for (auto &it: nxt) {
            it = nullptr;
        }
    }
};

using TrieNodePtr = TrieNode *;

template<int nodeN>
class AutoMaton {
    InfoStack &infoStack;

    /// ======= trie ========
    std::vector<TrieNodePtr> nodeSet;
    TrieNodePtr root = new TrieNode();

    /// ==== private method =====
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
        TrieNodePtr cur_node = root;
        for (auto it: str) {
            assert((int) it < 128);
            if (cur_node->nxt[(unsigned char) it] == nullptr) {
                cur_node->nxt[(int) it] = new TrieNode(it, cur_node->depth + 1);
                nodeSet.push_back(cur_node->nxt[(int) it]);
            }
            assert(cur_node != cur_node->nxt[(int) it]);
            cur_node = cur_node->nxt[(int) it];
        }
        if (cur_node->end == 0) {
            //如果str已经加进去了，cur_node的编号不为0
            cur_node->end = No;
        }
    }

    void build_AC() {
        std::queue<TrieNodePtr> Q;
        for (auto &root_son: root->nxt) {
            if (root_son != nullptr) {
                root_son->fail = root;
                Q.push(root_son);
            }
        }

        while (!Q.empty()) {
            auto frt = Q.front();
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
            //frt->last = frt->last && frt->last->end ? frt->fail : frt->fail->last;
            frt->last = frt->fail->end ? frt->fail : frt->fail->last;
        }
    }

    /// ==== match variable =====
    long long pt = 0;
    TrieNodePtr matchCur = root;//root == 1
    int endCnt = 0;
    int visitedCnt = 0;
public:
    explicit AutoMaton(InfoStack &infoSatck) : infoStack(infoSatck) {}

    ~AutoMaton() {
        delete root;
        for (auto it: nodeSet) {
            delete it;
        }
    }

    void match(const char &c) {
        matchCur = matchCur->nxt[uint8_t(c)] ? matchCur->nxt[uint8_t(c)] : root;
        for (auto it = matchCur; it != nullptr; it = it->last) {
            if (it->end) {
                infoStack.push_st(INFO{pt, 0, it->end - 1});
            }
        }
        ++pt;
    }


    void init(const std::vector<std::string> &strs) {
        nodeSet.clear();
        pt = 0;
        matchCur = root;//root == 1
        endCnt = 0;
        visitedCnt = 0;
        int i = 0;
        for (auto &s: strs) {
            trie_insert(s, ++i);
        }
//        std::cout << "The size of trie is " << nodeSet.size() + 1 << '\n';
        build_AC();
    }
};


}

namespace TrieNodeMyHeap {

struct FailEnd{

};
struct TrieNode {
    using TrieNodePtr = TrieNode *;
    TrieNodePtr nxt[128]{};
    TrieNodePtr fail;
    int depth;
    int end;
    TrieNodePtr last;
    char c;

    TrieNode() : fail(nullptr), depth(0), end(0), last(nullptr), c(0) {
        for (auto &it: nxt) {
            it = nullptr;
        }
    }

    TrieNode(char c, int depth) : fail(nullptr), depth(depth), end(0), last(nullptr), c(c) {
        for (auto &it: nxt) {
            it = nullptr;
        }
    }
};


using TrieNodePtr = TrieNode *;

template<int nodeN>
class AutoMaton {
    InfoStack &infoStack;

    /// ======= trie ========
    std::array<TrieNode, nodeN> nodeSet;
    int nodeCnt = 1;
    TrieNodePtr root = &nodeSet[0];

    /// ==== private method =====
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
        TrieNodePtr cur_node = root;
        for (auto it: str) {
            assert((int) it < 128);
            if (cur_node->nxt[(unsigned char) it] == nullptr) {
                nodeSet[++nodeCnt] = TrieNode(it, cur_node->depth + 1);
                cur_node->nxt[(int) it] = &nodeSet[nodeCnt];
            }
            assert(cur_node != cur_node->nxt[(int) it]);
            cur_node = cur_node->nxt[(int) it];
        }
        if (cur_node->end == 0) {
            //如果str已经加进去了，cur_node的编号不为0
            cur_node->end = No;
        }
    }

    void build_AC() {
        std::queue<TrieNodePtr> Q;
        for (auto &root_son: root->nxt) {
            if (root_son != nullptr) {
                root_son->fail = root;
                Q.push(root_son);
            }
        }

        while (!Q.empty()) {
            auto frt = Q.front();
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
            //frt->last = frt->last && frt->last->end ? frt->fail : frt->fail->last;
            frt->last = frt->fail->end ? frt->fail : frt->fail->last;
        }
    }

    /// ==== match variable =====
    long long pt = 0;
    TrieNodePtr matchCur = root;//root == 1
    int endCnt = 0;
    int visitedCnt = 0;
public:
    explicit AutoMaton(InfoStack &infoSatck) : infoStack(infoSatck) {}


    void match(const char &c) {
        matchCur = matchCur->nxt[uint8_t(c)] ? matchCur->nxt[uint8_t(c)] : root;
        for (auto it = matchCur; it != nullptr; it = it->last) {
            if (it->end) {
                infoStack.push_st(INFO{pt, 0, it->end - 1});
            }
        }
        ++pt;
    }


    void init(const std::vector<std::string> &strs) {
        nodeSet.fill(TrieNode());
        nodeCnt = 1;
        pt = 0;
        matchCur = root;//root == 1
        endCnt = 0;
        visitedCnt = 0;
        int i = 0;
        for (auto &s: strs) {
            trie_insert(s, ++i);
        }
//        std::cout << "The size of trie is " << nodeCnt << '\n';
        build_AC();
    }
};


}
}//end_namespace_AC





#endif //BUPT_FINAL_AC_H