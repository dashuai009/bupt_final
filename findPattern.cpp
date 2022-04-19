#include<bits/stdc++.h>

const int N = 1e6 + 10;
constexpr int M = 60;


void progressBar(double progress) {
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

namespace KMP {
void cal_next(const char *str, int *next, int len) {
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len - 1; q++) {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}

void match(const char *str, int slen, const char *ptr, int plen, const std::function<void(int)> &f) {
    int *next = new int[plen];
    cal_next(ptr, next, plen);//计算next数组
    int k = -1;
    for (int i = 0; i < slen; i++) {
        while (k > -1 && ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen - 1)//说明k移动到ptr的最末端
        {
            f(i);
            k = -1;//重新初始化，寻找下一个
            i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
        }
    }
    delete[] next;
}

}

void G(char *source, int sourceLen, char *dest, int &destLen, const char &flag) {
    static char cur[M + 1];
    static std::map<std::string, int> cnt;
    cnt.clear();
    for (int i = 0; i < sourceLen; ++i) {
        if (i % (std::max(sourceLen / 100, 1)) == 0) { progressBar(i * 1.0 / sourceLen); }
        for (int l = 3; l < 60; ++l) {
            if (l + i >= sourceLen)break;
            strncpy(cur, source + i, l);
            cur[l] = 0;
            std::string tmp = cur;
            if (cnt.find(tmp) != cnt.end()) {
                cnt[tmp] += l - 1;
            } else {
                cnt[tmp] = l - 1;
            }
        }
    }
    std::pair<std::string, int> bestSubStr{"", 0};
    for (auto it: cnt) {
        if (it.second > bestSubStr.second) {
            bestSubStr = it;
        }
    }
    std::cout << "找出的最大子串是：\n" << bestSubStr.first
              << "\n可以压缩的大小是" << bestSubStr.second << '\n';
//              << "占比 " << bestSubStr.second * 100.0 / sourceLen << " %\n";
    const int curLen = bestSubStr.first.length();
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
char s[N], t[N];

int main() {
    FILE *input = fopen(input_file, "r");
    fseek(input, 0, SEEK_END);
    auto input_size = ftell(input);
    rewind(input);
    fread(s, 1, input_size, input);
    fclose(input);
    std::cout << "the size of input:" << input_size << '\n';
    int slen = input_size;
    int tlen = 0;
    int flagPt = 128;
    for (int i = 0; i < 20; ++i) {
        ++flagPt;
        if (i & 1) {
            G(t, tlen, s, slen, (char) flagPt);
            std::cout << "after G:" << slen
                      << "\n新压缩的占比 " << (tlen - slen) * 100.0 / input_size << "%\n";
        } else {
            G(s, slen, t, tlen, (char) flagPt);
            std::cout << "after G:" << tlen
                      << "\n新压缩的占比 " << (slen - tlen) * 100.0 / input_size << "%\n";
        }
    }
//    for (int i = 0; i < tlen; ++i) {
//        if (t[i] == char(128)) {
//            std::cout << "   128\n";
//        } else {
//            std::cout << t[i];
//        }
//    }
    return 0;
}