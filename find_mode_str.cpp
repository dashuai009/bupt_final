#include<bits/stdc++.h>

const int N = 1e6 + 10;
char input_file[] = "data/test_in1.txt";
char s[N];
std::map<std::string, int> cnt;
using INFO = std::pair<int, std::string>;

struct myGreater : std::binary_function<INFO, INFO, bool> {
    bool operator()(const INFO &x, const INFO &y) const {
        return x.first * x.second.length() - x.first > y.first * y.second.length() - y.first;
    }
};

std::priority_queue<INFO, std::vector<INFO>, myGreater> qu;

int main() {
    FILE *input = fopen(input_file, "r");
    fseek(input, 0, SEEK_END);
    auto input_size = ftell(input);
    rewind(input);
    fread(s, 1, input_size, input);
    fclose(input);
    char cur[105];
    std::cout << input_size << '\n';

    for (int i = 0; i < input_size; ++i) {
        if (i % 10000 == 0) { std::cout << i << '\n'; }
        for (int l = 3; l < 60; ++l) {
            if (l + i >= input_size)continue;
            strncpy(cur, s + i, l);
            cur[l] = 0;
            std::string tmp = cur;
            if (cnt.find(tmp) != cnt.end()) {
                cnt[tmp]++;
            } else {
                cnt[tmp] = 1;
            }
        }
    }
    std::cout << cnt.size() << '\n';
    for (auto it: cnt) {
        while (qu.size() >= 200)qu.pop();
        qu.push({it.second, it.first});
    }
    while (!qu.empty()) {
        auto t = qu.top();
        qu.pop();
        std::cout << t.first << ' ' << t.second << '\n';
    }
    return 0;
}