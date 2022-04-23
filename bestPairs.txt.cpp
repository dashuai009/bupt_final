//
// Created by dashuai on 2022/4/15.
//

#include<bits/stdc++.h>

using Pair = std::pair<int, int>;
const int N = 1e6 + 10;

int f[N];
std::vector<Pair> p;

int main() {
    int s, t;
    while (std::cin >> s >> t) {
        p.emplace_back(s, t);
    }
    std::sort(p.begin(), p.end(), [](const Pair &a, const Pair &b) {
        return a.second < b.second || (a.second == b.second && a.first < b.first);
    });
    int j = 0;
    f[0] = 0;
    for (int i = 1; i < N; ++i) {
        f[i] = f[i - 1];
        while (j < p.size() && p[j].second == i) {
            f[i] = std::max(f[i], f[p[j].first - 1] + i - p[j].first);
            ++j;
        }
    }
    std::cout<<f[N-1]<<'\n';
    return 0;
}