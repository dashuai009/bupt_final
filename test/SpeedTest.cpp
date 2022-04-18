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
static void ZipSpeed(benchmark::State &state) {
    PatternStr::readPatternStr("data/defaultConfig.txt");
    static InfoStack infoStack(N*3, [](auto x) {});
    static AC::AutoMaton<400> autoMaton(infoStack);
    autoMaton.init(PatternStr::pattern_str);
    static InputBuffer inputBuffer(N,[](const char &x){autoMaton.match(x);});
    for (auto _: state) {
        inputBuffer.readData("data/test_in3.txt");
    }
}

BENCHMARK(ZipSpeed)->Iterations(30);

BENCHMARK_MAIN();