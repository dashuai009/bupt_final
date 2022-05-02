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
static const char zipOutFiles[5][20] = {
        "",
        "data/test_out1.txt",
        "data/test_out2.txt",
        "data/test_out3.txt",
        "data/test_out4.txt"
};
static const char unzipOutFile[] = "data/test_unzip.txt";

int64_t fileSize(const char *file_name) {
    auto input_ptr = fopen(file_name, "rb");
    if (!input_ptr) {
        Log(std::cerr, "input_File error!\n");
        return 0;
    }
    // obtain file size:
    fseek(input_ptr, 0, SEEK_END);
    auto fs = ftell(input_ptr);
    fclose(input_ptr);
    return fs;
}

using F = std::function<void(const char *inputFile, const char *)>;
auto BM_test = [](benchmark::State &state, const char *inputFile, const char *outputFile, const F &f) {
    const auto inFileSize = fileSize(inputFile);
    double zipSpeed = 0;
    for (auto _: state) {
        f(inputFile, outputFile);
        zipSpeed += inFileSize;
    }
    const auto outFileSize = fileSize(outputFile);
    state.counters["ratio"] = std::min(inFileSize, outFileSize) * 1.0 / std::max(inFileSize, outFileSize);
    state.counters["speed"] = benchmark::Counter(zipSpeed, benchmark::Counter::kIsRate);
};
static int iters[] = {0, 100, 20, 10, 5};
static int max_file_num = 4;

int main(int argc, char **argv) {
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))return 1;
    PatternStr::readPatternStr(config);
    char gzipCmd[6][100];
    char gzipOutFile[6][100];

    char ungzipCmd[6][100];
    for (int i = 1; i <= max_file_num; ++i) {
        sprintf(gzipCmd[i], "gzip -k -f %s", zipInFiles[i]);
        sprintf(gzipOutFile[i], "%s.gz", zipInFiles[i]);
        sprintf(ungzipCmd[i], "gzip -d -k -f %s.gz", zipInFiles[i]);
    }
    for (int i = 1; i <= max_file_num; ++i) {
        benchmark::RegisterBenchmark("  GzipTest", BM_test, zipInFiles[i], gzipOutFile[i],
                                     [i, gzipCmd](auto a, auto b) {
                                         system(gzipCmd[i]);
                                     })
                ->Unit(benchmark::kMillisecond)
                ->Iterations(iters[i])->Arg(i)
                ->UseRealTime();
    }

    for (int i = 1; i <= max_file_num; ++i) {
        benchmark::RegisterBenchmark("   zipTest", BM_test, zipInFiles[i], zipOutFiles[i], ZIP::compress)
                ->Unit(benchmark::kMillisecond)
                ->Iterations(iters[i])->Arg(i)
                ->UseRealTime();

    }

    for (int i = 1; i <= max_file_num; ++i) {
        benchmark::RegisterBenchmark("UnGzipTest", BM_test, gzipOutFile[i], zipInFiles[i],
                                     [i, ungzipCmd](auto a, auto b) {
                                         system(ungzipCmd[i]);
                                     })
                ->Unit(benchmark::kMillisecond)
                ->Iterations(iters[i])->Arg(i)
                ->UseRealTime();

    }
    for (int i = 1; i <= max_file_num; ++i) {
        benchmark::RegisterBenchmark(" unzipTest", BM_test, zipOutFiles[i], unzipOutFile, UNZIP::uncompress)
                ->Unit(benchmark::kMillisecond)
                ->Iterations(iters[i])->Arg(i)
                ->UseRealTime();
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}