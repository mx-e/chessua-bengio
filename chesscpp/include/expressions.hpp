#ifndef EXPRESSIONS
#define EXPRESSIONS

#include "types.hpp"
#include "constants.hpp"
#include "utils.hpp"

inline uint64_t get_pieces(const C_BoardState &board, const float color, const uint8_t piece_type)
{
    return board.pieces[piece_type] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_pawns(const C_BoardState &board, const float color)
{
    return board.pieces[b_pawns] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_rooks(const C_BoardState &board, const float color)
{
    return board.pieces[b_rooks] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_knights(const C_BoardState &board, const float color)
{
    return board.pieces[b_knights] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_bishops(const C_BoardState &board, const float color)
{
    return board.pieces[b_bishops] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_queen(const C_BoardState &board, const float color)
{
    return board.pieces[b_queens] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_king(const C_BoardState &board, const float color)
{
    return board.pieces[b_kings] & board.pieces[color_to_BB_index.at(color)];
};

inline uint64_t get_pieces_of_color(const C_BoardState &board, const float color)
{
    return board.pieces[color_to_BB_index.at(color)];
}

inline uint64_t get_all_pieces(const C_BoardState &board)
{
    return board.pieces[b_white] | board.pieces[b_black];
}

inline uint64_t get_empty_fields(const C_BoardState &board)
{
    return ~(get_all_pieces(board));
}

inline uint64_t get_enemy_fields(const C_BoardState &board, const float color)
{
    return get_pieces_of_color(board, color * -1);
}

inline uint64_t get_empty_or_enemy(const C_BoardState &board, const float color)
{
    return get_empty_fields(board) | get_enemy_fields(board, color);
}

inline uint64_t get_pawn_single_moves(const C_BoardState &board, const float color)
{
    return mask_and_shift_by_n(single, get_pawns(board, color), color) & get_empty_fields(board);
}

inline uint64_t get_pawn_double_moves(const C_BoardState &board, const uint64_t single_moves, const float color)
{
    uint64_t half = color == 1. ? white_half : black_half;
    return mask_and_shift_by_n(single, single_moves & center_half, color) & get_empty_fields(board) & half;
}

inline uint64_t get_pawn_attacks(const C_BoardState &board, const pawn_shift direction, const float color)
{
    return mask_and_shift_by_n(direction, get_pawns(board, color), color);
}

inline uint64_t get_knight_moves(const int field_idx)
{
    return knight_moves[field_idx];
}

inline uint64_t get_king_moves(const int field_idx)
{
    return king_moves[field_idx];
}

inline uint64_t get_bishop_moves_and_attacks(int field_idx, uint64_t blockers)
{
    uint64_t moves = empty_board;
    // North West
    moves |= rays[NW][field_idx];
    if (rays[NW][field_idx] & blockers)
    {
        int blockerIndex = forward_scan(rays[NW][field_idx] & blockers);
        moves &= ~rays[NW][blockerIndex];
    }

    // North East
    moves |= rays[NE][field_idx];
    if (rays[NE][field_idx] & blockers)
    {
        int blockerIndex = reverse_scan(rays[NE][field_idx] & blockers);
        moves &= ~rays[NE][blockerIndex];
    }

    // South East
    moves |= rays[SE][field_idx];
    if (rays[SE][field_idx] & blockers)
    {
        int blockerIndex = reverse_scan(rays[SE][field_idx] & blockers);
        moves &= ~rays[SE][blockerIndex];
    }

    // South West
    moves |= rays[SW][field_idx];
    if (rays[SW][field_idx] & blockers)
    {
        int blockerIndex = forward_scan(rays[SW][field_idx] & blockers);
        moves &= ~rays[SW][blockerIndex];
    }
    return moves;
};

inline uint64_t get_rook_moves_and_attacks(int field_idx, uint64_t blockers)
{
    uint64_t moves = empty_board;
    // North
    moves |= rays[N][field_idx];
    if (rays[N][field_idx] & blockers)
    {
        int blockerIndex = reverse_scan(rays[N][field_idx] & blockers);
        moves &= ~rays[N][blockerIndex];
    }

    // East
    moves |= rays[E][field_idx];
    if (rays[E][field_idx] & blockers)
    {
        int blockerIndex = reverse_scan(rays[E][field_idx] & blockers);
        moves &= ~rays[E][blockerIndex];
    }

    // South
    moves |= rays[S][field_idx];
    if (rays[S][field_idx] & blockers)
    {
        int blockerIndex = forward_scan(rays[S][field_idx] & blockers);
        moves &= ~rays[S][blockerIndex];
    }

    // West
    moves |= rays[W][field_idx];
    if (rays[W][field_idx] & blockers)
    {
        int blockerIndex = forward_scan(rays[W][field_idx] & blockers);
        moves &= ~rays[W][blockerIndex];
    }
    return moves;
}

#endif
