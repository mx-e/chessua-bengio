#ifndef BB_UTILS
#define BB_UTILS
#include <iostream>
#include <map>
#include <bit>
#include <list>
#include <stdint.h>
#include "bitboard.hpp"
#include "bitboard_constants.hpp"

const std::map<float, uint64_t> shift_mask_map = {
    {10., ~(row_7 | row_8)},
    {17., ~row_8},
    {15., ~row_1},
    {6., ~(row_1 | row_2)},
    {-10., ~(row_1 | row_2)},
    {-17., ~row_1},
    {-15, ~row_8},
    {-6, ~(row_7 | row_8)}};

inline uint64_t shift_by_n(const float shift, const uint64_t val)
{
    return shift > 0 ? val << (int)shift : val >> (int)(shift * -1.);
}

inline uint64_t mask_and_shift_by_n(const float shift, const uint64_t val)
{
    uint64_t shift_mask = shift_mask_map.at(shift);
    return shift_by_n(shift, val & shift_mask);
}

inline int forward_scan(const uint64_t bb)
{
    uint64_t ls1b = bb & -bb; // isolate LS1B
    return std::countl_zero(ls1b);
}

inline int reverse_scan(const uint64_t bb)
{
    return std::countl_zero(bb);
}

inline std::list<int> scan_board(uint64_t bb)
{
    std::list<int> idx_list = {};
    if (bb)
        do
        {
            idx_list.push_back(forward_scan(bb));
        } while (bb &= bb - 1);
    return idx_list;
}

inline std::list<int> scan_board_reverse(uint64_t bb)
{
    std::list<int> idx_list = {};
    int reverse_scan_idx = 0;
    if (bb)
        do
        {
            reverse_scan_idx = reverse_scan(bb);
            idx_list.push_back(reverse_scan_idx);
        } while (bb &= ~(most_sig_bit >> reverse_scan_idx));
    return idx_list;
}

#endif