#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard.hpp"
#include "../include/bitboard_utils.hpp"
#include "../include/main.hpp"

#include "devutils.hpp"

int main()
{
    C_Board other_board = bb_create_board(get_example_board());
    print_bitboard(other_board.get_king_moves_and_attacks(other_board.turn));
    print_bitboard(other_board.get_empty_or_enemy(White));
    print_bitboard(other_board.get_king_moves_and_attacks(White));
    UCIStrings move_list = bb_generate_moves(get_example_board(), 1, {}, 1, 1, 1, 1, 0, 0);

    for (std::string m : move_list)
    {
        std::cout << m << "\n";
    }
}
