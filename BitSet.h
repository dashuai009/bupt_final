//
// Created by dashuai on 2021/11/21.
//

#ifndef BUPT_FINAL_BITSET_H
#define BUPT_FINAL_BITSET_H
/**
 * if Block is uint8_t, Rshift == 3, (x >> Rshift) is equal to (x / 8)
 * if Block is uint64_t, Rshift== 6, (x >> Rshift) is equal to (x / 64)
 */
#include <vector>

#define Rshift (3)
#define LastBlockBits (Size & Ones)
#define LastBlockNum (Size >> Rshift)

//Recommend Block's type:uint8_t,uint64_t
template<typename Block>
struct BitSet {
#define Ones (((Block)1<<Rshift)-1)
#define BlockBits ((Block)1 << Rshift)
    std::vector<Block> bits{};
    uint32_t Size = 0;

    uint32_t size() {
        return Size;
    }

    //void Set(unsigned int pos, bool val) {
    //val == 0 ? (s[pos >> Rshift] &= ~(1ull << (pos & ones))) : (s[pos >> Rshift] |= 1ull << (pos & ones));
    //}

    void push_back(const bool &val) {
        if (Size == 0 || LastBlockBits == 0) {
            bits.push_back(Block(0));
        }
        bits[LastBlockNum] <<= 1;
        bits[LastBlockNum] |= val;
        ++Size;
    }

    void push_back(const Block &x) {
        if (LastBlockBits) {
            bits[LastBlockNum] <<= (BlockBits - LastBlockBits);
            bits[LastBlockNum] |= (x >> LastBlockBits);
            bits.push_back(x & ((1 << LastBlockBits) - 1));
        } else {
            bits.push_back(x);
        }
        Size += (1 << Rshift);
    }

    void pop_back() {
        if (LastBlockBits == 1) {
            bits.pop_back();
        } else {
            bits[(Size - 1) >> Rshift] >>= 1;
        }
        --Size;
    }

    void append(const BitSet<Block> &x) {
        if ((Size & Ones) == 0) {//this 是整Blocks
            bits.insert(bits.end(), x.bits.begin(), x.bits.end());
            Size += x.Size;
            return;
        }
        for (int i = 0; i < (int(x.Size) >> 3)  ; ++i) {
            push_back(x.bits[i]);
        }
        if(x.Size &Ones){
            if ((Size & Ones) + (x.Size & Ones) <= (1 << Rshift)) {
                bits[LastBlockNum] <<= (x.Size & Ones);
                bits[LastBlockNum] |= (x.bits[x.Size >> Rshift]);
            } else {
                bits[LastBlockNum] <<= (BlockBits - LastBlockBits);
                bits[LastBlockNum] |=
                        x.bits[x.Size >> Rshift] >> ((Size & Ones) + (x.Size & Ones) - BlockBits);//右移位数==最后block的bit数
                bits.push_back(x.bits[x.Size >> Rshift] & ((1 << ((Size + x.Size) & Ones)) - 1));
            }
            Size += (x.Size & Ones);
        }

    }

    bool Test(unsigned int pos) const {
        if ((LastBlockBits) && (pos >= Size - LastBlockBits)) {
            return bits[pos >> Rshift] >> (LastBlockBits - 1 - (pos & Ones)) & 1;
        } else {
            return bits[pos >> Rshift] >> (BlockBits - 1 - (pos & Ones)) & 1;
        }
    }
};

#endif //BUPT_FINAL_BITSET_H
