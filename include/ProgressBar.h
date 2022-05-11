//
// Created by dashuai on 2022/5/2.
//

#ifndef BUPT_FINAL_PROGRESSBAR_H
#define BUPT_FINAL_PROGRESSBAR_H

const char VmRSS[] = "VmRSS:";

/* 打印当前消耗内存 */
std::string get_memory_by_pid(int pid) {
    FILE *stream = nullptr;
    char cache[256];
    char proc_pid_status[64];

    sprintf(proc_pid_status, "/proc/%d/status", pid);
    stream = fopen(proc_pid_status, "r");

    while (fgets(cache, 256, stream) != nullptr) {
        if (strstr(cache, VmRSS) != nullptr) {
            auto res = std::string{cache};
            res.erase(res.length()-1);
            return res;
        }
    }
}
/**
 * 反复刷新进度条，进度条宽度为70，需要显示的字符数不同时刷新进度条。而不是达到100%
 * @param progress 进度
 * @param total 总数
 * @param info 提示信息
 */
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
    std::cout << "] " << int(progress / total * 100.0) << " %"<<get_memory_by_pid(getpid())<<'\r';
    std::cout.flush();
}

#endif //BUPT_FINAL_PROGRESSBAR_H
