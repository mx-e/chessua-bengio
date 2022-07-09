#include <stdint.h>
#include <string>
#include <iostream>
#include <inttypes.h>
#include "../include/constants.hpp"
#include "../include/types.hpp"
#include "../include/session.hpp"

void print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int row = 0; row < 8; row++)
    {
        for (int col = 7; col >= 0; col--)
        {
            uint64_t index = 1;
            std::string bit = bb & (index << (col * 8 + row)) ? "1" : "∙";
            bb_str += bit;
            if (col != 0)
            {
                bb_str += " ";
            }
        }
        bb_str += '\n';
    }
    bb_str += '\n';
    std::cout << bb_str;
}

void print_bitboard_hex(uint64_t bb)
{
    printf("0x%" PRIx64 "LL,\n", bb);
}

void bitboard_table_from_logic(bool(logic)(int col, int row, int c, int r, float color), float color)
{
    for (int i = 0; i < 64; i++)
    {
        uint64_t bb = 0;
        uint8_t row = i % 8;
        uint8_t col = (uint8_t)i / 8;
        for (int j = 0; j < 64; j++)
        {
            uint8_t r = j % 8;
            uint8_t c = (uint8_t)j / 8;
            if (logic(col, row, c, r, color))
            {
                bb = set_board_1_at_idx(bb, j);
            }
        }
        print_bitboard_hex(bb);
    }
}
