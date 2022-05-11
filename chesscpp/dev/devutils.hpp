#ifndef DEVUTILS_H
#define DEVUTILS_H
#include <stdint.h>
#include <string>

void print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
        }
        bb_str += '\n';
    }
}

#endif