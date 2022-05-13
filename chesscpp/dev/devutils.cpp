#include <stdint.h>
#include <string>
#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard_types.hpp"

void print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
            uint64_t index = 1;
            std::string bit = bb & (index << (63 - (row * 8 + col))) ? "1" : "∙";
            bb_str += bit;
            if (row != 7)
            {
                bb_str += " ";
            }
        }
        bb_str += '\n';
    }
    bb_str += '\n';
    std::cout << bb_str;
}

C_Board get_new_game_board()
{
    C_Board board = C_Board();
    board.set_pieces(White, Pawn, white_pawns_starting_config);
    board.set_pieces(Black, Pawn, black_pawns_starting_config);
    board.set_pieces(White, Rook, white_rooks_starting_config);
    board.set_pieces(Black, Rook, black_rooks_starting_config);
    board.set_pieces(White, Knight, white_knights_starting_config);
    board.set_pieces(Black, Knight, black_knights_starting_config);
    board.set_pieces(White, Bishop, white_bishops_starting_config);
    board.set_pieces(Black, Bishop, black_bishops_starting_config);
    board.set_pieces(White, Queen, white_queen_starting_config);
    board.set_pieces(Black, Queen, black_queen_starting_config);
    board.set_pieces(White, King, white_king_starting_config);
    board.set_pieces(Black, King, black_king_starting_config);
    return board;
}