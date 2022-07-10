#ifndef B_TRANSFORM
#define B_TRANSFORM

#include "constants.hpp"
#include "types.hpp"
#include "session.hpp"
#include "expressions.hpp"
#include "utils.hpp"
#include "collections.hpp"
#include "pstables.hpp"

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
        uint64_t rook_move = castling_king_dest_to_rook_move_map.at(m.dest);
        board.pieces[b_rooks] ^= rook_move;
        board.pieces[color_to_BB_index.at(board.turn)] ^= rook_move;
    }
    if (m.ep)
    {
        unset_single_piece(board, board.turn * -1, pPawn, m.dest - (int)board.turn);
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
        uint64_t rook_move = castling_king_dest_to_rook_move_map.at(m.dest);
        board.pieces[b_rooks] ^= rook_move;
        board.pieces[color_to_BB_index.at(board.turn * -1)] ^= rook_move;
    }
    if (m.ep)
    {
        set_single_piece(board, board.turn, pPawn, m.dest + (int)board.turn);
    }
}

inline void update_castling_rights(C_BoardState &board)
{
    uint64_t king_and_rooks = get_rooks(board, board.turn) | get_king(board, board.turn);
    std::array<uint64_t, 2> castling_masks = color_to_castling_mask.at(board.turn);
    std::array<castling, 2> castling_indicator = color_to_castling_indicator.at(board.turn);

    bool castling_voided_ks = (bool)(((king_and_rooks & ~col_a) ^ castling_masks[0]));
    bool castling_voided_qs = (bool)(((king_and_rooks & ~col_h) ^ castling_masks[1]));

    if (castling_voided_ks)
    {
        unset_castling_rights(board, castling_indicator[0]);
    }
    if (castling_voided_qs)
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

    // update ps tables
    // update_score(Make, OPENING, board.ps_score[color_to_pstable_idx.at(board.turn)][OPENING], board.ps_score[color_to_pstable_idx.at(-board.turn)][OPENING], board, m);
    // update_score(Make, ENDGAME, board.ps_score[color_to_pstable_idx.at(board.turn)][ENDGAME], board.ps_score[color_to_pstable_idx.at(-board.turn)][ENDGAME], board, m);

    board.turn *= -1;

    collect_legal_moves(board, move_list);
    board.king_attack = (bool)(board.all_attacks & get_king(board, -1. * board.turn));
    board.castling_move_illegal = check_castling_move_illegal(board, m, board.all_attacks);

    board.move_stack.push_back(m);
}

inline move pop_move(C_BoardState &board)
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

    // update ps tables
    // update_score(Unmake, OPENING, board.ps_score[color_to_pstable_idx.at(board.turn)][OPENING], board.ps_score[color_to_pstable_idx.at(-board.turn)][OPENING], board, m);
    // update_score(Unmake, ENDGAME, board.ps_score[color_to_pstable_idx.at(board.turn)][ENDGAME], board.ps_score[color_to_pstable_idx.at(-board.turn)][ENDGAME], board, m);

    board.king_attack = false;
    board.castling_move_illegal = false;
    board.move_stack.pop_back();
    return m;
}

inline bool check_move_causes_check(C_BoardState &board, move &m, MoveList &move_list)
{
    push_move(board, m, move_list);
    bool check = board.king_attack || board.castling_move_illegal;
    pop_move(board);
    return check;
}

#endif