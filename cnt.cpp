#include <bits/stdc++.h>

int cnt[512];
std::vector<std::string> res;
std::string tot = "";
int main() {
    char c;
    int flag = false;
    std::string cur = "";
    while ((c = getchar()) != EOF) {
        cnt[(int)c]++;
    }
    for (int i = 0; i < 512; ++i) {
        if (cnt[i] == 0) {
            continue;
        }
        if (32 <= i && i < 127) { //可见字符
            std::cout << char(i) << ' ' << cnt[i] << '\n';
        } else {
            std::cout << i << ' ' << cnt[i] << '\n';
        }
    }
    std::priority_queue<long long, std::vector<long long>,
                        std::greater<long long>>
        Huffman;
    for (int i = 0; i < 512; ++i) {
        Huffman.push(cnt[i]);
        //     std::cout << i << ' ' << ppp[i] << '\n';
    }
    long long sum = 0;
    while (Huffman.size() >= 2) {
        long long top1 = Huffman.top();
        Huffman.pop();
        long long top2 = Huffman.top();
        Huffman.pop();
        //std::cout << top1 << ' ' << top2 << '\n';
        Huffman.push(top1 + top2);
        sum+=top1+top2;
    }
    std::cout << "Huffman size:" << sum << '\n';
    // std::cout<<sum<<'\n';
    // std::cout<<res.size()<<'\n';
    // std::cout<<res[0]<<' '<<res[1]<<'\n';
    // std::cout << tot;
    // for (auto it : res) {
    //     std::cerr << it << '\n';
    // }
    return 0;
}
