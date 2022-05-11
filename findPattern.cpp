#include<bits/stdc++.h>
#include "include/ProgressBar.h"
#include "include/KMP.h"

constexpr int M = 100;

void G(char *source, int sourceLen, char *dest, int &destLen, const char &flag) {
    static std::map<std::string_view, int> cnt;

    std::pair<std::string, int> bestSubStr{"", 0};
    int kkk = 0;
    for (int l = 3; l < M; ++l) {
        cnt.clear();
        for (int i = 0; i + l - 1 < sourceLen; ++i) {
            progressBar(++kkk, sourceLen * M, "find_substr");
            std::string_view tmp(source + i, l);
            cnt.find(tmp) != cnt.end() ? cnt[tmp] += l - 1 : cnt[tmp] = l - 1;
        }
        for (auto it: cnt) {
            if (it.second > bestSubStr.second) {
                bestSubStr = it;
            }
        }
    }

    std::cout << "The Longest substr is:\n" << bestSubStr.first
              << "\nSigma g(t) is equal to " << bestSubStr.second << '\n';
    const int curLen = bestSubStr.first.length();
    static char cur[M + 1];
    for (int i = 0; i < curLen; ++i) {
        cur[i] = bestSubStr.first[i];
    }
    int lastMatchPos = -1;
    int pt = 0;
    KMP::match(source, sourceLen, cur, curLen, [&](const int x) {
        for (int i = lastMatchPos + 1; i < x - curLen; ++i) {
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

int main() {
    FILE *input = fopen(input_file, "r");
    fseek(input, 0, SEEK_END);
    auto input_size = ftell(input);
    rewind(input);
    char *s = new char[input_size + 10], *t = new char[input_size + 10];
    fread(s, 1, input_size, input);
    fclose(input);
    std::cout << "the size of input:" << input_size << '\n';
    int slen = static_cast<int>(input_size), tlen = 0, flagPt = 128;
    for (int i = 0; i < 20; ++i) {
        G(s, slen, t, tlen, (char) flagPt++);
        std::cout << "after G:" << tlen
                  << "\n|S'|-|S|=" << (slen - tlen) * 100.0 / input_size << "%\n";
        std::swap(s, t);
        slen = tlen;
    }
    delete[]s;
    delete[]t;
    return 0;
}