
#include <iostream>
#include <vector>
#include <string>
#include "board.hpp"
#include "utils.hpp"

int main()
{
    std::string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board chess_board = Board();

    chess_board.set_board_from_fen(fen_board);
    chess_board.print_board();

    return 0;
}