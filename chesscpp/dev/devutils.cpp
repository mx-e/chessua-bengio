#include <stdint.h>
#include <string>
#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard.hpp"

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

C_Board get_new_game_board()
{
    C_Board board = C_Board();
    board.set_pieces(White, pPawn, white_pawns_starting_config);
    board.set_pieces(Black, pPawn, black_pawns_starting_config);
    board.set_pieces(White, pRook, white_rooks_starting_config);
    board.set_pieces(Black, pRook, black_rooks_starting_config);
    board.set_pieces(White, pKnight, white_knights_starting_config);
    board.set_pieces(Black, pKnight, black_knights_starting_config);
    board.set_pieces(White, pBishop, white_bishops_starting_config);
    board.set_pieces(Black, pBishop, black_bishops_starting_config);
    board.set_pieces(White, pQueen, white_queen_starting_config);
    board.set_pieces(Black, pQueen, black_queen_starting_config);
    board.set_pieces(White, pKing, white_king_starting_config);
    board.set_pieces(Black, pKing, black_king_starting_config);
    return board;
}

Board get_example_board()
{
    Board board = Board();
    board[0] = {5, 6, 0, 0, 0, 0, -6, -5};
    board[1] = {4, 6, 0, 0, 0, 0, -6, -4};
    board[2] = {3, 6, 0, 0, 0, 0, -6, -3};
    board[3] = {2, 6, 0, 0, 0, 0, -6, -2};
    board[4] = {1, 6, 0, 0, 0, 0, -6, -1};
    board[5] = {3, 6, 0, 0, 0, 0, -6, -3};
    board[6] = {4, 6, 0, 0, 0, 0, -6, -4};
    board[7] = {5, 6, 0, 0, 0, 0, -6, -5};
    return board;
}
