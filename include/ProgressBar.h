//
// Created by dashuai on 2022/5/2.
//

#ifndef BUPT_FINAL_PROGRESSBAR_H
#define BUPT_FINAL_PROGRESSBAR_H

void progressBar(double progress, double total, const char *info) {
    constexpr int barWidth = 70;
    static int pos = -1;
    if (progress < 0 || progress > total || int(barWidth * progress / total) == pos)return;
    pos = barWidth * progress / total;
    std::cout << info << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress / total * 100.0) << " %\r";
    std::cout.flush();
}

#endif //BUPT_FINAL_PROGRESSBAR_H
