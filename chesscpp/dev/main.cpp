#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard.hpp"
#include "../include/bitboard_utils.hpp"
#include "../include/bitboard_moves.hpp"
#include "devutils.hpp"

int main()
{
    C_Board board = get_new_game_board();
    print_bitboard(board.get_pawns(White));
    print_bitboard(board.get_pawns(Black));
    print_bitboard(board.pieces[0]);
    print_bitboard(board.get_all_pieces());
    print_bitboard(board.get_empty_fields());
    print_bitboard(board.get_pawn_attacks(Black));
    print_bitboard(board.get_pawn_double_moves(Black));
    print_bitboard(board.get_pawn_single_moves(Black));
    print_bitboard(board.get_knight_moves_and_attacks(White));
    print_bitboard(board.get_knight_moves_and_attacks(Black));
    print_bitboard(king_moves[32]);
    uint64_t moves = king_moves[32];
    std::list<int> l = scan_board(moves);
    for (int i : l)
    {
        std::cout << field_id_to_string(i) << "\n";
    }
    print_bitboard(board.get_king_moves_and_attacks(Black));
    std::list<int> t = scan_board_reverse(moves);
    for (int i : t)
    {
        std::cout << field_id_to_string(i) << "\n";
    }
}
