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
};