#ifndef BB_MOVES
#define BB_MOVES
#include <stdint.h>
#include <string>
#include <map>

enum move_flags
{
    empty,
    queen_promotion,
    bishop_promotion,
    knight_promotion,
    rook_promotion,
};

enum move_captures
{
    none,
    king,
    queen,
    bishop,
    knight,
    rook,
    pawn,
};
const std::map<int8_t, char>
    move_flag_to_char_map = {{empty, ' '}, {queen_promotion, 'q'}, {bishop_promotion, 'b'}, {knight_promotion, 'n'}, {rook_promotion, 'r'}};
const std::map<int, char>
    col_int_to_string_map = {{0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'}};

struct move
{
    uint8_t piece;
    uint8_t start;
    uint8_t end;
    uint8_t capture;
    uint8_t flag;
};

inline move create_move(const uint8_t piece, const u_int8_t start, const uint8_t end, const move_captures capture = none, const move_flags flag = empty)
{
    struct move m;
    m.piece = piece;
    m.start = start;
    m.end = end;
    m.flag = flag;
    return m;
}

inline std::string field_id_to_string(uint8_t idx)
{
    int col = (int)idx / 8;
    int row = 8 - (idx % 8);
    return col_int_to_string_map.at(col) + std::to_string(row);
}

inline std::string move_to_uci_move(const move m)
{
    char flag = move_flag_to_char_map.at(m.flag);
    return field_id_to_string(m.start) + field_id_to_string(m.end) + flag;
}

#endif