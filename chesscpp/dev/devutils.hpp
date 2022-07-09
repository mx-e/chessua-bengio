#ifndef DEVUTILS_H
#define DEVUTILS_H

void print_bitboard(uint64_t bb);
void print_all_knight_move_boards();
void bitboard_table_from_logic(bool(logic)(int col, int row, int c, int r, float color), float color);

inline C_BoardState get_new_game_board()
{
    C_BoardState board;
    uint64_t black_pieces = black_king_starting_config | black_queen_starting_config | black_bishops_starting_config | black_knights_starting_config | black_rooks_starting_config | black_pawns_starting_config;
    uint64_t white_pieces = white_king_starting_config | white_queen_starting_config | white_bishops_starting_config | white_knights_starting_config | white_rooks_starting_config | white_pawns_starting_config;

    board.pieces[b_black] = black_pieces;
    board.pieces[b_white] = white_pieces;

    board.pieces[b_kings] = black_king_starting_config | white_king_starting_config;
    board.pieces[b_queens] = black_queen_starting_config | white_queen_starting_config;
    board.pieces[b_bishops] = black_bishops_starting_config | white_bishops_starting_config;
    board.pieces[b_knights] = black_knights_starting_config | white_knights_starting_config;
    board.pieces[b_rooks] = black_rooks_starting_config | white_rooks_starting_config;
    board.pieces[b_pawns] = black_pawns_starting_config | white_pawns_starting_config;

    return board;
}
#endif