#ifndef EXTRACTIONS
#define EXTRACTIONS

#include "types.hpp"
#include "session.hpp"
#include "utils.hpp"

inline void extract_moves_with_offset(uint64_t move_board, MoveList &move_list, float source_offset)
{
    if (move_board)
    {
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m));
        } while (move_board &= move_board - 1);
    }
}

inline void extract_pawn_double_moves(uint64_t move_board, MoveList &move_list, float color)
{
    if (move_board)
    {
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move(m - (int)color * 2., (uint8_t)m, 0, 0, 0, 0, m - (int)color));
        } while (move_board &= move_board - 1);
    }
}

inline void extract_promotions(uint64_t move_board, MoveList &move_list, float source_offset)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            for (uint8_t p : {2, 3, 4, 5})
            {

                move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, 0, p));
            }
        } while (move_board &= move_board - 1);
}

inline void extract_ep_captures(uint64_t move_board, MoveList &move_list, float source_offset)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, 0, 0, 0, 1));
        } while (move_board &= move_board - 1);
}

inline void extract_moves_with_explicit_src(uint64_t move_board, MoveList &move_list, int src_idx)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move(src_idx, (uint8_t)m));
        } while (move_board &= move_board - 1);
}

inline void extract_captures_with_offset(const C_BoardState &board_state, uint64_t move_board, MoveList &move_list, float source_offset)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move(m + (int)source_offset, m, get_piece_type_of_field(board_state, (uint8_t)m)));
        } while (move_board &= move_board - 1);
}

inline void extract_promotion_captures(const C_BoardState &board_state, uint64_t move_board, MoveList &move_list, float source_offset)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            for (uint8_t p : {2, 3, 4, 5})
            {
                move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, get_piece_type_of_field(board_state, m), p));
            }

        } while (move_board &= move_board - 1);
}

inline void extract_captures_with_explicit_src(const C_BoardState &board_state, uint64_t move_board, MoveList &move_list, int src_idx)
{
    if (move_board)
        do
        {
            int m = forward_scan(move_board);
            move_list.push_back(create_move((uint8_t)src_idx, (uint8_t)m, get_piece_type_of_field(board_state, m)));
        } while (move_board &= move_board - 1);
}

#endif