//
// Created by dashuai on 2021/11/18.
//

#ifndef BUPT_FINAL_LOG_H
#define BUPT_FINAL_LOG_H
#include<iostream>
bool quiet = true;

template<typename T>
std::ostream &Log(std::ostream &os, const T &t) {
    if(!quiet) {
        return os << t << '\n';
    }else{
        return os;
    }
}

template<typename T, typename... Args>
std::ostream &Log(std::ostream &os, const T &t, const Args &...rest) {
    if(!quiet) {
        os << t << ' ';
        return Log(os, rest...);
    }else{
        return os;
    }
}

#endif //BUPT_FINAL_LOG_H

