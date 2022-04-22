#include "board.hpp"
#include "utils.hpp"
#include <string>
#include <vector>
#include <iostream>

Board::Board()
{
    reset_board();
}

Board::~Board()
{
}

void Board::reset_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            if (j == 6)
            {
                set_tile(i, j + 1, black_pawn);
            }
            else if (j == 1)
            {
                set_tile(i, j + 1, white_pawn);
            }
            else
            {
                set_tile(i, j + 1, empty);
            }
    }
    set_tile(a, 8, black_rook);
    set_tile(h, 8, black_rook);

    set_tile(b, 8, black_knight);
    set_tile(g, 8, black_knight);

    set_tile(c, 8, black_bishop);
    set_tile(f, 8, black_bishop);

    set_tile(d, 8, black_queen);
    set_tile(e, 8, black_king);

    set_tile(a, 8, white_rook);
    set_tile(h, 8, white_rook);

    set_tile(b, 8, white_knight);
    set_tile(g, 8, white_knight);

    set_tile(c, 8, white_bishop);
    set_tile(f, 8, white_bishop);

    set_tile(d, 8, white_queen);
    set_tile(e, 8, white_king);

    to_move = white;
    can_castle[white_kingside] = true;
    can_castle[white_queenside] = true;
    can_castle[black_kingside] = true;
    can_castle[black_queenside] = true;
    en_passant_tile[0] = -1;
    en_passant_tile[1] = -1;
    reversible_halfmoves_clock = 0;
    move_clock = 0;
}

void Board::set_tile(int tile_col, int tile_row, int new_state)
{
    tiles[tile_col][tile_row - 1] = new_state;
}

int Board::get_tile(int tile_col, int tile_row)
{
    return tiles[tile_col][tile_row - 1];
}

void Board::set_board_from_fen(std::string fen)
{
    reset_board();
    std::vector<std::string> fen_sections = split_string(fen, " ");
    std::vector<std::string> board_rows = split_string(fen_sections[0], "/");

    // extract tile states
    for (int row = 0; row < board_rows.size(); row++)
    {
        int col = 1;
        for (int char_idx = 0; char_idx < board_rows[row].length(); char_idx++)
        {
            char current_char = board_rows[row][char_idx];
            int board_state = fen_char_to_board_state(current_char);
            if (board_state != -1)
            {
                set_tile(col, row, board_state);
                col++;
            }
            else
            {
                int digit = char_to_int(current_char);
                for (int i = 0; i < digit; i++)
                {
                    set_tile(col, row, empty);
                    col++;
                }
            }
        }
    }

    // extract to_move
    if (fen_sections[1] == "w")
    {
        to_move = white;
    }
    else
    {
        to_move = black;
    }

    // set can_castle
    for (int i = 0; i < sizeof(can_castle); i++)
    {
        can_castle[i] = false;
    }
    for (int char_idx = 0; char_idx < fen_sections[2].length(); char_idx++)
    {
        char current_char = fen_sections[2][char_idx];
        int castling_indicator_idx = fen_castling_char_to_idx(current_char);
        if (castling_indicator_idx == -1)
        {
            break;
        }
        else
        {
            can_castle[castling_indicator_idx] = true;
        }
    }
    // set en_passant_tile
    if (fen_sections[3] != "-")
    {
        char col = fen_sections[3][0];
        char row = fen_sections[3][1];

        en_passant_tile[0] = abc_to_123(col);
        en_passant_tile[1] = char_to_int(row);
    }
    // set reversible_halfmoves_clock and moves_clock
    int halfmoves = std::stoi(fen_sections[4]);
    int moves = std::stoi(fen_sections[5]);

    reversible_halfmoves_clock = halfmoves;
    move_clock = moves;
}

void Board::print_board()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            std::cout << board_state_to_char(get_tile(col, row));
        }
        std::cout << std::endl;
    }
}