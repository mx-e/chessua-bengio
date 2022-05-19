#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard.hpp"
#include "../include/bitboard_utils.hpp"
#include "../include/main.hpp"

#include "devutils.hpp"

int main()
{
    ///////CONSTANT DEBUG
    // C_Board other_board = bb_create_board(get_example_board());
    // for (uint64_t r : rows)
    // {
    //     print_bitboard(r);
    // }
    // for (uint64_t c : cols)
    // {
    //     print_bitboard(c);
    // }
    // print_bitboard(white_squares);
    // print_bitboard(black_squares);
    // print_bitboard(whole_field);
    // print_bitboard(white_half);
    // print_bitboard(black_half);
    // print_bitboard(center_half);
    // print_bitboard(edge_half);
    // print_bitboard(most_sig_bit);
    // print_bitboard(empty_board);

    // int i = 0;
    // for (uint64_t m : king_moves)
    // {
    //     std::cout << i << "\n";
    //     print_bitboard(m);
    //     i++;
    // }

    // UCIStrings move_list = bb_generate_moves(get_example_board(), 1, {}, 1, 1, 1, 1, 0, 0);

    // C_Board test = get_new_game_board();
    // print_bitboard(test.get_pawn_double_moves(test.turn));
    // print_bitboard(test.get_pawn_single_moves(White));

    // int i = 0;
    // for (uint64_t bb : rays[7])
    // {
    //     std::cout << i << "\n";
    //     print_bitboard(bb);
    //     i++;
    // }

    C_Board board = mailbox_to_bitboard_representation(get_example_board());
    board.collect_legal_moves();
    auto legal_moves = board.legal_moves;
    move &m;
    for (move mv : legal_moves)
    {
        if (mv.src == 8)
        {
            m = mv;
        }
    }

    // print_all_knight_move_boards();
}
