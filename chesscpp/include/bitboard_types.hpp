#ifndef BB_TYPES
#define BB_TYPES
#include <stdint.h>
#include <map>

class C_Board
{
public:
    uint64_t pieces[8];
    enum enumBoards
    {
        b_black,
        b_kings,
        b_queens,
        b_bishops,
        b_knights,
        b_rooks,
        b_pawns,
        b_white,
    };
    const std::map<int, int> color_to_BB_index = {{-1, 0}, {1, 7}};

    inline uint64_t get_pieces(int color, int type)
    {
        return pieces[type] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_pawns(int color)
    {
        return pieces[b_pawns] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_rooks(int color)
    {
        return pieces[b_rooks] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_knights(int color)
    {
        return pieces[b_knights] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_bishops(int color)
    {
        return pieces[b_bishops] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_queen(int color)
    {
        return pieces[b_queens] & pieces[color_to_BB_index.at(color)];
    };
    inline uint64_t get_king(int color)
    {
        return pieces[b_kings] & pieces[color_to_BB_index.at(color)];
    };
    inline void set_pieces(int color, int type, uint64_t new_pieces)
    {
        uint64_t other_color_pieces = get_pieces(color * -1, type);
        uint64_t own_color_pieces = get_pieces(color, type);
        uint64_t all_own_color_pieces_minus_type = pieces[color_to_BB_index.at(color)] ^ own_color_pieces;
        pieces[type] = other_color_pieces | new_pieces;
        pieces[color_to_BB_index.at(color)] = all_own_color_pieces_minus_type | new_pieces;
    }
};
#endif