//
// Created by dashuai on 2022/4/16.
//

#include <benchmark/benchmark.h>
#include<iostream>
#include <include/PatternStr.h>
#include <include/InputBuffer.h>
#include "include/AC.h"
#include "include/INFO.h"

#define N 128*1024

// zip
static void ZipSpeedHeap(benchmark::State &state) {
    PatternStr::readPatternStr("data/defaultConfig.txt");
    static InfoStack infoStack(N*3, [](auto x) {});
    static AC::TrieNodeHeap::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData("data/test_in3.txt");
    }
}

// zip
static void ZipSpeedArray(benchmark::State &state) {
    PatternStr::readPatternStr("data/defaultConfig.txt");
    static InfoStack infoStack(N*3, [](auto x) {});
    static AC::TrieNodeArray::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData("data/test_in3.txt");
    }
}
// zip
static void ZipSpeedMyHeap(benchmark::State &state) {
    PatternStr::readPatternStr("data/defaultConfig.txt");
    static InfoStack infoStack(N*3, [](auto x) {});
    static AC::TrieNodeMyHeap::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData("data/test_in3.txt");
    }
}
const int iters = 20;
BENCHMARK(ZipSpeedHeap)->Iterations(iters);
BENCHMARK(ZipSpeedMyHeap)->Iterations(iters);
BENCHMARK(ZipSpeedArray)->Iterations(iters);

BENCHMARK_MAIN();