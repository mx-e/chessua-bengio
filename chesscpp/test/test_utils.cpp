#include "test_utils.hpp"

void fill_bitboard(uint64_t &bitboard, std::vector<std::pair<int, int>> positions)
{
    for (auto position : positions)
    {
        uint8_t idx = flat(position.first, position.second);
        bitboard |= (most_sig_bit >> idx);
    }
}

bool exists(std::vector<T> iterable, std::function<bool(T)> condition)
{
    bool found;
    for (auto item : iterable)
    {
        found = found || condition(item);
    }
    return found;
}