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
    char config[] = "data/defaultConfig.txt";
    char inFile[] = "data/test_in3.txt";
    inFile[12] = char('0' + state.range(0));
    PatternStr::readPatternStr(config);
     InfoStack infoStack(N*3, [](auto x) {});
     AC::TrieNodeHeap::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[&autoMaton](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData(inFile);
    }
    PatternStr::pattern_str.clear();
}

// zip
static void ZipSpeedArray(benchmark::State &state) {
    char config[] = "data/defaultConfig.txt";
    char inFile[] = "data/test_in3.txt";
    inFile[12] = char('0' + state.range(0));
    PatternStr::readPatternStr(config);
     InfoStack infoStack(N*3, [](auto x) {});
     AC::TrieNodeArray::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[&autoMaton](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData(inFile);
    }
    PatternStr::pattern_str.clear();
}
// zip
static void ZipSpeedMyHeap(benchmark::State &state) {
    char config[] = "data/defaultConfig.txt";
    char inFile[] = "data/test_in3.txt";
    inFile[12] = char('0' + state.range(0));
    PatternStr::readPatternStr(config);
    InfoStack infoStack(N*3, [](auto x) {});
    AC::TrieNodeMyHeap::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[&autoMaton](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData(inFile);
    }
    PatternStr::pattern_str.clear();
}
const int iters1 = 200;
BENCHMARK(ZipSpeedHeap)->Iterations(iters1)->Arg(1);
BENCHMARK(ZipSpeedMyHeap)->Iterations(iters1)->Arg(1);
BENCHMARK(ZipSpeedArray)->Iterations(iters1)->Arg(1);

const int iters2 = 100;
BENCHMARK(ZipSpeedHeap)->Iterations(iters2)->Arg(2);
BENCHMARK(ZipSpeedMyHeap)->Iterations(iters2)->Arg(2);
BENCHMARK(ZipSpeedArray)->Iterations(iters2)->Arg(2);

const int iters3 = 20;
BENCHMARK(ZipSpeedHeap)->Iterations(iters3)->Arg(3);
BENCHMARK(ZipSpeedMyHeap)->Iterations(iters3)->Arg(3);
BENCHMARK(ZipSpeedArray)->Iterations(iters3)->Arg(3);

BENCHMARK_MAIN();