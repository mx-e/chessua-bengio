#include <cstdint>
#include <string>

class Board
{
    int8_t tiles[8][8];
    bool to_move;
    bool can_castle[4];
    int en_passant_tile[2];
    int reversible_halfmoves_clock;
    int move_clock;

public:
    Board();
    ~Board();
    void set_tile(int tile_col, int tile_row, int new_state);
    int get_tile(int tile_col, int tile_row);
    void reset_board();

    // FEN strings are a way to represent the state of a chess game.
    // Example: r3k2r/pp1n2pp/2p2q2/b2p1n2/BP1Pp3/P1N2P2/2PB2PP/R2Q1RK1 w kq b3 0 13
    void set_board_from_fen(std::string fen);
    void print_board();
};