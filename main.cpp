#include <bits/stdc++.h>
#include "include/SetOptions.h"
#include "include/Zip.h"


int main(int argc, char *argv[]) {
    auto beginTime = std::chrono::system_clock::now();
    set_options(argc, argv);
    PatternStr::readPatternStr(config_file);
    if (mode == 0) {
        ZIP::compress(input_file, output_file);
    } else {
        UNZIP::uncompress(input_file, output_file);
    }
    auto endTime = std::chrono::system_clock::now();
    auto dur = std::chrono::duration<double,std::milli>(endTime - beginTime);
    std::cout <<"cost "<< dur.count() << " ms\n";
    return 0;
}
