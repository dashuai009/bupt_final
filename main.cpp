#include <bits/stdc++.h>
#include "include/AC.h"
#include "include/Buffer.h"
#include "include/CircularQueue.h"
#include "include/CompressBuffer.h"
#include "include/Huffman.h"
#include "include/Log.h"
#include "include/SetOptions.h"
#include "include/OutputBuffer.h"
#include "include/InputBuffer.h"
#include "include/INFO.h"
#include "include/Zip.h"
const int32_t N = 128 * 1024;


void unzip(char target_file[],  char output_file[]) {
    FILE *target = fopen(target_file, "r");

    OutputBuffer out_stream(N);
    out_stream.setOutputFile(output_file);
    UnzipMatchBuffer UM(N * 10, [&out_stream](const uint8_t &c) {
        out_stream.push(c);
    });
//    HuffmanDecode HD = HuffmanDecode(N * 50, [&UM](const uint8_t &c) {// 这地方写的有点hack
//        UM.push(c);
//    });
    int c;
    while ((c = getc(target)) != EOF) {
        UM.push(c);
    }
    //UM.clear_and_do();
    out_stream.clear_and_do();
    fclose(target);
}


int main(int argc, char *argv[]) {
    auto beginTime = std::chrono::system_clock::now();
    set_options(argc, argv);
    PatternStr::readPatternStr(config_file);
    if (mode == 0) {
        ZIP::compress(input_file, output_file);
    } else {
        unzip(input_file, output_file);
    }
    auto endTime = std::chrono::system_clock::now();
    auto dur = std::chrono::duration<double,std::milli>(endTime - beginTime);
    std::cout <<"cost "<< dur.count() << " ms\n";
    return 0;
}
