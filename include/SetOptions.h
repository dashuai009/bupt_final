//
// Created by dashuai on 2021/11/24.
//

#ifndef BUPT_FINAL_SETOPTIONS_H
#define BUPT_FINAL_SETOPTIONS_H

#include<getopt.h>

#ifndef BUPT_FINAL_LOG_H
#include "Log.h"
extern bool quiet;
#endif
const char help_text[] =
        "只可接受可见字符(ascii码为32~127)和'\\n'"
        "字符的纯文本文件。可接受如下参数列表：\n"
        "-h\t\t\t可选，显示此帮助信息\n"
        "-i <input.file>\t\tt必选，输入文件\n"
        "-o <output.file>\t可选，默认覆盖原文件\n"
        "-p <config.txt>\t\t可选，配置文件，默认提供defalutConfig.txt文件\n"
        "-d\t\t\t解压模式\n"
        "-q\t\t\t可选，安静模式\n";
//定义长选项
static struct option long_options[] = {
        {"help",       no_argument,       nullptr, 'h'},
        {"version",    no_argument,       nullptr, 'v'},
        {"input",      required_argument, nullptr, 'i'},
        {"output",     required_argument, nullptr, 'o'},
        {"config",     required_argument, nullptr, 'p'},
        {"quiet",      no_argument,       nullptr, 'q'},
        {"decompress", no_argument,       nullptr, 'd'}
};
char input_file[100] = "data/test_in.txt";
char output_file[100] = "data/test_out.txt";
char config_file[100] = "data/defaultConfig.txt";
int mode = 0;

void set_options(int argc, char *argv[]) {
    int opt = 0;
    int index = 0;
    while ((opt = getopt_long(argc, argv, "h::i:o:p:d::v:", long_options, &index)) != -1) {
        switch (opt) {
            case 'h':
                printf("%s", help_text);
                exit(0);
                break;
            case 'i':
                strcpy(input_file, optarg);
                if (strlen(output_file) == 0) {
                    strcpy(output_file, input_file);
                }
                break;
            case 'o':
                strcpy(output_file, optarg);
                output_file[strlen(optarg)] = 0;
                break;
            case 'p':
                strcpy(config_file, optarg);
                config_file[strlen(optarg)] = 0;
                break;
            case 'q':
                quiet = true;
                break;
            case 'd':
                mode = 1; //解压模式
                break;
            default:
                break;
        }
    }
    Log(std::cout, "input_file:  ", input_file);
    Log(std::cout, "output_file: ", output_file);
    Log(std::cout, "config_file: ", config_file);
}

#endif //BUPT_FINAL_SETOPTIONS_H
