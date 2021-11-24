//
// Created by dashuai on 2021/11/21.
//

#ifndef BUPT_FINAL_BITSET_H
#define BUPT_FINAL_BITSET_H
using T = uint8_t;
#define Rshift ((sizeof(T)*3))
#define ones ((1<<Rshift)-1)

struct BitSet {
    uint8_t s[512 >> Rshift];
    unsigned int Size = 0;

    BitSet() {
        memset(s, 0, sizeof(s));
    }

    //void Set(unsigned int pos, bool val) {
    //val == 0 ? (s[pos >> Rshift] &= ~(1ull << (pos & ones))) : (s[pos >> Rshift] |= 1ull << (pos & ones));
    //}

    void push_back(bool val) {
        s[Size >> Rshift] <<= 1;
        s[Size >> Rshift] |= val;
        ++Size;
    }

    void pop_back() {
        s[(Size - 1) >> Rshift] >>= 1;
        --Size;
    }

    bool Test(unsigned int pos) const {
        if ((Size & 7) && (pos >= Size - (Size & 7))) {
            return s[pos >> Rshift] >> ((Size & 7) - 1 - (pos & 7)) & 1;
        } else {
            return s[pos >> Rshift] >> (7 - (pos & 7)) & 1;
        }
    }
};

#endif //BUPT_FINAL_BITSET_H
