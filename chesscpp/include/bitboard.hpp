#ifndef BB
#define BB
#include <stdint.h>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <cmath>
#include "bitboard_types.hpp"
#include "bitboard_constants.hpp"
#include "types.hpp"
#include "bitboard_utils.hpp"
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
enum enumCastling
{
    w_kingside,
    w_queenside,
    b_kingside,
    b_queenside,
};

enum enumEnPassant
{
    en_passant_col,
    en_passant_row
};

class C_Board
{
public:
    uint64_t pieces[8];

    uint8_t castling[4];

    uint8_t en_passant[2];

    float turn = 1.;
    float moves = 0.;
    float half_moves = 0.;

    std::vector<move> legal_moves = {};

    const std::map<float, int>
        color_to_BB_index = {{-1., 0}, {1., 7}};

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

    inline void set_single_piece(const float color, const uint8_t type, const uint8_t position_idx)
    {
        pieces[type] |= (most_sig_bit >> position_idx);
        pieces[color_to_BB_index.at(color)] |= (most_sig_bit >> position_idx);
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
        float shift = -1. * color;
        uint64_t no_promotion_mask = color == 1. ? row_7 : row_2;
        uint64_t pawns = get_pawns(color) & no_promotion_mask;
        return mask_and_shift_by_n(shift, pawns) & get_empty_fields();
    }

    inline uint64_t get_pawn_promotions(const float color)
    {
        float shift = -1. * color;
        uint64_t promotion_mask = color == 1. ? row_7 : row_2;
        uint64_t pawns = get_pawns(color) & promotion_mask;
        return mask_and_shift_by_n(shift, pawns) & get_empty_fields();
    }

    inline uint64_t get_pawn_promotion_attacks_left(const float color)
    {
        float shift = -7. * color;
        uint64_t promotion_mask = color == 1. ? row_7 : row_2;
        uint64_t pawns = get_pawns(color) & promotion_mask;
        return mask_and_shift_by_n(shift, pawns) & get_empty_fields();
    }

    inline uint64_t get_pawn_promotion_attacks_right(const float color)
    {
        float shift = 9. * color;
        uint64_t promotion_mask = color == 1. ? row_7 : row_2;
        uint64_t pawns = get_pawns(color) & promotion_mask;
        return mask_and_shift_by_n(shift, pawns) & get_empty_fields();
    }

    inline uint64_t get_pawn_double_moves(const float color)
    {
        uint64_t half = color == 1. ? white_half : black_half;
        float shift = 1. * color;
        uint64_t single_moves = get_pawn_single_moves(color) & center_half;
        return shift_by_n(shift, single_moves) & get_empty_fields() & half;
    }

    inline uint64_t get_pawn_attacks_left(const float color)
    {
        float shift_left_attack = -7 * color;
        int color_idx = color_to_BB_index.at(color * -1);
        return mask_and_shift_by_n(shift_left_attack, get_pawns(color)) & pieces[color_idx];
    }

    inline uint64_t get_pawn_attacks_right(const float color)
    {
        float shift_right_attack = 9. * color;
        int color_idx = color_to_BB_index.at(color * -1);
        return mask_and_shift_by_n(shift_right_attack, get_pawns(color)) & pieces[color_idx];
    }
    inline void collect_pawn_moves_and_captures();

    inline uint64_t get_knight_moves_and_attacks(const float color, const knight_direction dir)
    {
        uint64_t knights = get_knights(color);
        return mask_and_shift_by_n(knight_idx_offsets.at(dir), knights) & get_empty_or_enemy(color);
    }
    inline void collect_knight_moves_and_captures();

    inline uint64_t get_king_moves_and_attacks(const float color)
    {
        int king_pos = scan_board(get_king(color)).front();
        return king_moves[king_pos] & get_empty_or_enemy(color);
    }

    inline void collect_king_moves_and_captures();

    inline void collect_legal_moves();
};

inline C_Board mailbox_to_bitboard_representation(Board mailbox)
{
    C_Board bitboard = C_Board();
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
            int field_state = mailbox[col][row];
            if (field_state == 0)
            {
                continue;
            }
            float color = (float)((0. < field_state) - (field_state < 0.));
            uint8_t type = abs(field_state);
            uint8_t position_idx = row_col_idx_to_position_idx(col, row);
            bitboard.set_single_piece(color, type, position_idx);
        }
    }
    return bitboard;
}

uint8_t get_piece_type_of_field(const C_Board *board, int position_idx);

void extract_moves_with_offset(uint64_t move_board, const std::vector<move> &move_list, int source_offset);

void extract_captures_with_offset(const C_Board *board, uint64_t move_board, const std::vector<move> &move_list, int source_offset);

void extract_promotions(uint64_t move_board, std::vector<move> &move_list);

void extract_promotion_captures(const C_Board *board, uint64_t move_board, const std::vector<move> &move_list);

void extract_moves_with_explicit_src(uint64_t move_board, const std::vector<move> &move_list, int src_idx);

void extract_captures_with_explicit_src(const C_Board &board, uint64_t move_board, const std::vector<move> &move_list, int src_idx);

UCIStrings get_uci_moves(const C_Board board);

#endif