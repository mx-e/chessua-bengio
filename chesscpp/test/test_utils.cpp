#include "test_utils.hpp"

void fill_bitboard(uint64_t &bitboard, std::vector<std::pair<int, int>> positions)
{
    for (auto position : positions)
    {
        uint8_t idx = flat(position.first, position.second);
        bitboard |= (most_sig_bit >> idx);
    }
}

uint64_t fill_bitboard_max(uint64_t bitboard, std::vector<int> positions)
{
    for (auto position : positions)
    {
        bitboard = bitboard | (most_sig_bit >> position);
    }
    return bitboard;
}

int flat(int row, int col)
{
    return row * 8 + col;
}
