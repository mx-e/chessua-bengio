#ifndef B_TRANSFORM
#define B_TRANSFORM

#include "constants.hpp"
#include "types.hpp"
#include "expressions.hpp"
#include "utils.hpp"
#include "collections.hpp"

inline void set_single_piece(C_BoardState &board, const float color, const uint8_t type, const uint8_t position_idx)
{
    board.pieces[type] |= (most_sig_bit >> position_idx);
    board.pieces[color_to_BB_index.at(color)] |= (most_sig_bit >> position_idx);
}

inline void unset_single_piece(C_BoardState &board, const float color, const uint8_t type, const uint8_t position_idx)
{
    board.pieces[type] &= ~(most_sig_bit >> position_idx);
    board.pieces[color_to_BB_index.at(color)] &= ~(most_sig_bit >> position_idx);
}

inline void set_pieces(C_BoardState &board, const float color, const uint8_t type, uint64_t new_pieces)
{
    uint64_t other_color_pieces = get_pieces(board, color * -1, type);
    uint64_t own_color_pieces = get_pieces(board, color, type);
    uint64_t all_own_color_pieces_minus_type = get_pieces_of_color(board, color) ^ own_color_pieces;
    board.pieces[type] = other_color_pieces | new_pieces;
    board.pieces[color_to_BB_index.at(color)] = all_own_color_pieces_minus_type | new_pieces;
}

inline void set_castling_rights(C_BoardState &board, castling c_type)
{
    uint8_t address_bit = 1 << c_type;
    board.castling_rights |= address_bit;
}

inline void unset_castling_rights(C_BoardState &board, castling c_type)
{
    board.castling_rights &= ~castling_to_castling_state_mask.at(c_type);
}

inline void execute_move_forward(C_BoardState &board, const move &m)
{
    uint8_t piece_type = get_piece_type_of_field(board, m.src);
    unset_single_piece(board, board.turn, piece_type, m.src);
    if (m.capture)
    {
        unset_single_piece(board, board.turn * -1, m.capture, m.dest);
    }
    if (m.promotion)
    {
        piece_type = m.promotion;
    }
    set_single_piece(board, board.turn, piece_type, m.dest);
    if (m.castling)
    {
        if (m.castling == 1)
        {
            unset_single_piece(board, board.turn, pRook, 56);
            set_single_piece(board, board.turn, pRook, 40);
        }
        else if (m.castling == 2)
        {
            unset_single_piece(board, board.turn, pRook, 0);
            set_single_piece(board, board.turn, pRook, 24);
        }
    }
    if (m.ep)
    {
        unset_single_piece(board, board.turn, m.capture, m.dest - (int)board.turn);
    }
}

inline void execute_move_backward(C_BoardState &board, const move &m)
{
    uint8_t piece_type = get_piece_type_of_field(board, m.dest);
    unset_single_piece(board, board.turn * -1, piece_type, m.dest);
    if (m.capture)
    {
        set_single_piece(board, board.turn, m.capture, m.dest);
    }
    if (m.promotion)
    {
        piece_type = pPawn;
    }
    set_single_piece(board, board.turn * -1, piece_type, m.src);
    if (m.castling)
    {
        if (m.castling == 1)
        {
            unset_single_piece(board, board.turn, pRook, 40);
            set_single_piece(board, board.turn, pRook, 56);
        }
        else if (m.castling == 2)
        {
            unset_single_piece(board, board.turn, pRook, 23);
            set_single_piece(board, board.turn, pRook, 0);
        }
    }
    if (m.ep)
    {
        set_single_piece(board, board.turn, m.capture, m.dest + (int)board.turn);
    }
}

inline void update_castling_rights(C_BoardState &board)
{
    uint64_t king_and_rooks = get_rooks(board, board.turn) | get_king(board, board.turn);
    std::array<uint64_t, 2> castling_masks = color_to_castling_mask.at(board.turn);
    std::array<castling, 2> castling_indicator = color_to_castling_indicator.at(board.turn);

    bool ks_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[0]));
    bool qs_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[1]));

    if (!ks_castling_rights)
    {
        unset_castling_rights(board, castling_indicator[0]);
    }
    if (!qs_castling_rights)
    {
        unset_castling_rights(board, castling_indicator[1]);
    }
}

inline bool check_castling_move_illegal(C_BoardState &board, const move &m, const uint64_t all_attacks)
{
    std::array<uint64_t, 2> no_attack_masks = color_to_castling_no_attack.at(board.turn * -1);
    bool match_kingside = (bool)(all_attacks & no_attack_masks[0]);
    bool match_queenside = (bool)(all_attacks & no_attack_masks[1]);
    return (m.castling == 1 && match_kingside) || (m.castling == 2 && match_queenside);
}

inline void push_move(C_BoardState &board, move m, MoveList &move_list)
{
    execute_move_forward(board, m);

    m.prev_c = board.castling_rights;
    update_castling_rights(board);

    uint8_t ep_field = m.ep_field;
    m.ep_field = board.en_passant;
    board.en_passant = ep_field;

    // set counters
    uint8_t piece_type = get_piece_type_of_field(board, m.dest);
    m.prev_half_move_c = (uint8_t)board.half_moves;
    board.half_moves += 1.;
    board.half_moves *= ((!(piece_type == pPawn) && !(m.capture)));
    board.moves += (board.turn == -1.) * 1.;

    board.turn *= -1;

    move_list.clear();
    collect_legal_moves(board, move_list);
    board.king_attack = (bool)(board.all_attacks & get_king(board, -1. * board.turn));
    board.castling_move_illegal = check_castling_move_illegal(board, m, board.all_attacks);

    board.move_stack.push_back(m);
}

inline move pop_move(C_BoardState &board, MoveList &move_list)
{
    if (board.move_stack.empty())
    {
        return create_move(0, 0);
    }
    move m = board.move_stack.back();

    execute_move_backward(board, m);

    board.castling_rights = m.prev_c;

    uint8_t ep_field = m.ep_field;
    m.ep_field = board.en_passant;
    board.en_passant = ep_field;

    // set_counters
    board.half_moves = m.prev_half_move_c;
    board.moves -= (board.turn == 1.) * 1.;

    board.turn *= -1;
    board.king_attack = false;
    board.castling_move_illegal = false;
    board.move_stack.pop_back();
    return m;
}

inline bool check_move_causes_check(C_BoardState &board, move &m, MoveList &move_list)
{
    push_move(board, m, move_list);
    bool check = board.king_attack;
    pop_move(board, move_list);
    return check;
}

#endif