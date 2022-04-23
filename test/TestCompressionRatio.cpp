//
// Created by dashuai on 2022/4/23.
//
#include <benchmark/benchmark.h>
#include <include/PatternStr.h>
#include "include/Zip.h"

static const char zipInFiles[5][20] = {
        "",
        "data/test_in1.txt",
        "data/test_in2.txt",
        "data/test_in3.txt",
        "data/test_in4.txt"
};
static const char config[] = "data/defaultConfig.txt";
static const char zipOutFile[] = "data/test_out.txt";
static const char unzipOutFile[] = "data/test_unzip.txt";

// zip
static void ZipTest(benchmark::State &state) {
    PatternStr::readPatternStr(config);
    for (auto _: state) {
        ZIP::compress(zipInFiles[state.range(0)], zipOutFile);
    }
    PatternStr::pattern_str.clear();
}

// unzip
static void UnZipTest(benchmark::State &state) {
    PatternStr::readPatternStr(config);
    for (auto _: state) {
        UNZIP::uncompress(zipOutFile, unzipOutFile);
    }
    PatternStr::pattern_str.clear();
}


//const int iters1 = 100;
//BENCHMARK(ZipTest)->Iterations(iters1)->Arg(1);
//BENCHMARK(UnZipTest)->Iterations(iters1)->Arg(1);
//
//
//const int iters2 = 10;
//BENCHMARK(ZipTest)->Iterations(iters2)->Arg(2);
//BENCHMARK(UnZipTest)->Iterations(iters2)->Arg(2);
//
//
//const int iters3 = 5;
//BENCHMARK(ZipTest)->Iterations(iters3)->Arg(3);
//BENCHMARK(UnZipTest)->Iterations(iters3)->Arg(3);

//
//const int iters4 = 2;
//BENCHMARK(ZipTest)->Iterations(iters4)->Arg(3);
//BENCHMARK(UnZipTest)->Iterations(iters4)->Arg(3);

static int iters[] = {0, 20, 20, 10, 2};

int main(int argc, char **argv) {
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))return 1;
    for (int i = 1; i <= 4; ++i) {
        benchmark::RegisterBenchmark("ZipTest", ZipTest)->Iterations(iters[i])->Arg(i);
        benchmark::RegisterBenchmark("UnZipTest", UnZipTest)->Iterations(iters[i])->Arg(i);
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}