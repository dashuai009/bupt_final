//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_LOG_H
#define BUPT_FINAL_LOG_H
bool quiet = false;

template<typename T>
std::ostream &Log(std::ostream &os, const T &t) {
    return os << t << '\n';
}

template<typename T, typename... Args>
std::ostream &Log(std::ostream &os, const T &t, const Args &...rest) {
    os << t << ' ';
    return Log(os, rest...);
}

#endif //BUPT_FINAL_LOG_H

