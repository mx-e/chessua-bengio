#ifndef BB
#define BB
#include <stdint.h>
#include <map>
#include <iostream>
#include "bitboard_utils.hpp"
#include "bitboard_constants.hpp"

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

    uint8_t castling[4];
    enum enumCastling
    {
        w_kingside,
        w_queenside,
        b_kingside,
        b_queenside,
    };

    uint8_t en_passant[2];
    enum enumEnPassant
    {
        en_passant_col,
        en_passant_row
    };

    float turn;
    float moves;
    float half_moves;

    const std::map<float, int> color_to_BB_index = {{-1., 0}, {1., 7}};

    inline uint64_t get_pieces(const float color, const uint8_t type)
    {
        return pieces[type] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_pawns(const float color)
    {
        return pieces[b_pawns] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_rooks(const float color)
    {
        return pieces[b_rooks] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_knights(const float color)
    {
        return pieces[b_knights] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_bishops(const float color)
    {
        return pieces[b_bishops] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_queen(const float color)
    {
        return pieces[b_queens] & pieces[color_to_BB_index.at(color)];
    };

    inline uint64_t get_king(const float color)
    {
        return pieces[b_kings] & pieces[color_to_BB_index.at(color)];
    };

    inline void set_pieces(const float color, const uint8_t type, uint64_t new_pieces)
    {
        uint64_t other_color_pieces = get_pieces(color * -1, type);
        uint64_t own_color_pieces = get_pieces(color, type);
        uint64_t all_own_color_pieces_minus_type = pieces[color_to_BB_index.at(color)] ^ own_color_pieces;
        pieces[type] = other_color_pieces | new_pieces;
        pieces[color_to_BB_index.at(color)] = all_own_color_pieces_minus_type | new_pieces;
    }

    inline uint64_t get_all_pieces()
    {
        return pieces[b_white] | pieces[b_black];
    }

    inline uint64_t get_empty_fields()
    {
        return ~(get_all_pieces());
    }

    inline uint64_t get_empty_or_enemy(float color)
    {
        return get_empty_fields() | pieces[color_to_BB_index.at(color * -1)];
    }

    inline uint64_t get_pawn_single_moves(const float color)
    {
        float shift = 1. * color;
        return shift_by_n(shift, get_pawns(color)) & get_empty_fields();
    }

    inline uint64_t get_pawn_promotions(const float color)
    {
        uint64_t row = color == 1. ? row_8 : row_1;
        return get_pawn_single_moves(color) & row;
    }

    inline uint64_t get_pawn_double_moves(const float color)
    {
        uint64_t half = color == 1. ? white_half : black_half;
        float shift = 1. * color;
        uint64_t single_moves = get_pawn_single_moves(color) & center_half;
        return shift_by_n(shift, single_moves) & get_empty_fields() & half;
    }

    inline uint64_t get_pawn_attacks(const float color)
    {
        float shift_right_attack = 9. * color;
        float shift_left_attack = -7 * color;
        int color_idx = color_to_BB_index.at(color * -1);
        uint64_t right_attacks = shift_by_n(shift_right_attack, get_pawns(color)) & pieces[color_idx];
        uint64_t left_attacks = shift_by_n(shift_left_attack, get_pawns(color)) & pieces[color_idx];
        return right_attacks | left_attacks;
    }

    inline uint64_t get_knight_moves_and_attacks(const float color)
    {
        uint64_t knights = get_knights(color);
        uint64_t knight_moves = empty_board;
        const float knight_shifts[] = {10., 17., 15., 6., -10., -17., -15., -6.};
        for (int n : knight_shifts)
        {
            knight_moves |= mask_and_shift_by_n(n, knights);
        }
        return knight_moves & get_empty_or_enemy(color);
    }

    inline uint64_t get_king_moves_and_attacks(const float color)
    {
        int king_pos = scan_board(get_king(color)).front();
        return king_moves[king_pos] & get_empty_or_enemy(color);
    }
};

#endif