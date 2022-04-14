//
// Created by dashuai on 2022/4/14.
//

#ifndef BUPT_FINAL_PATTERNSTR_H
#define BUPT_FINAL_PATTERNSTR_H
#include<vector>

namespace PatternStr {
using STR = std::string;
std::vector <STR> pattern_str;

void readPatternStr(char config_file[]) {
    FILE *config = fopen(config_file, "r");
    assert(config != nullptr);
    std::string s;
    int c;
    while ((c = getc(config)) != EOF) {
        if (c == '\n') {
            if (s.length() > 0) {
                pattern_str.push_back(s);
            }
            s = "";
        } else {
            s += char(c);
        }
    }
    fclose(config);
}

};

#endif //BUPT_FINAL_PATTERNSTR_H
