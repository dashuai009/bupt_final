#include<bits/stdc++.h>
#include "include/ProgressBar.h"
#include "include/KMP.h"

constexpr int M = 100;

std::vector<std::string> subStrSet;

std::string changeSubStr(const std::string &sub) {
    std::string res;
    for (auto it: sub) {
        if (int(it) < 0) {
            res += subStrSet[-it];
        } else {
            res += it;
        }
    }
    return res;
}

void G(char *source, int sourceLen, char *dest, int &destLen, const char &flag) {
    static std::unordered_map<std::string_view, int> cnt;

    std::pair<std::string, int> bestSubStr{"", 0};
    int kkk = 0;
    for (int l = 3; l < M; ++l) {
        cnt.clear();
        progressBar(l, M, "find_substr");
        for (int i = 0; i + l - 1 < sourceLen; ++i) {
            std::string_view tmp(source + i, l);
            cnt.find(tmp) != cnt.end() ? cnt[tmp] += l - 1 : cnt[tmp] = l - 1;
        }
        for (auto &it: cnt) {
            if (it.second > bestSubStr.second) {
                bestSubStr = it;
            }
        }
    }
    subStrSet.push_back(changeSubStr(bestSubStr.first));

    std::cout << "The Longest substr is:\n" << subStrSet.back()
              << "\nSigma g(t) is equal to " << bestSubStr.second << '\n';
    const int curLen = bestSubStr.first.length();
    static char cur[M + 1];
    for (int i = 0; i < curLen; ++i) {
        cur[i] = bestSubStr.first[i];
    }
    int lastMatchPos = -1;
    int pt = 0;
    KMP::match(source, sourceLen, cur, curLen, [&](const int x) {
        for (int i = lastMatchPos + 1; i < x - curLen + 1; ++i) {
            dest[pt++] = source[i];
        }
        if (lastMatchPos < x - curLen + 1) {
            dest[pt++] = flag;
            lastMatchPos = x;
        }
    });
    for (int i = lastMatchPos + 1; i < sourceLen; ++i) {
        dest[pt++] = source[i];
    }
    destLen = pt;
}

char input_file[] = "data/test_in1.txt";
char config3txt[] = "data/config3.txt";

int main() {
    FILE *input = fopen(input_file, "r");
    fseek(input, 0, SEEK_END);
    auto input_size = ftell(input);
    rewind(input);
    char *s = new char[input_size + 10], *t = new char[input_size + 10];
    fread(s, 1, input_size, input);
    fclose(input);
    std::cout << "the size of input:" << input_size << '\n';
    int slen = static_cast<int>(input_size), tlen = 0;
    char flagPt = 0;
    for (int i = 0; i < 40; ++i) {
        G(s, slen, t, tlen, flagPt--);
        std::cout << "after G:" << tlen
                  << "\n|S'|-|S|=" << (slen - tlen) * 100.0 / input_size << "%\n";
        std::swap(s, t);
        slen = tlen;
    }
    delete[]s;
    delete[]t;
    FILE *fb = freopen(config3txt, "wb", stdout);
    for (auto it: subStrSet) {
        std::cout << it << '\n';
    }
    fclose(fb);
    return 0;
}