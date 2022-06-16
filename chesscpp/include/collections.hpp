#ifndef COLLECTIONS
#define COLLECTIONS

#include "types.hpp"
#include "expressions.hpp"
#include "extractions.hpp"

inline bool get_castling_possible(C_BoardState &board, castling c_type)
{
    uint64_t castling_free = castling_to_castling_free.at(c_type);
    uint8_t state_mask = castling_to_castling_state_mask.at(c_type);
    return (state_mask & board.castling_rights) && ((get_all_pieces(board) & castling_free) == 0);
}

inline void collect_pawn_moves(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t promotion_row = color_to_promotion_row.at(board_state.turn);
    uint64_t pawn_single_moves = get_pawn_single_moves(board_state, board_state.turn);

    extract_promotions(pawn_single_moves & promotion_row, legal_moves, -pawn_shift_map.at(single) * board_state.turn);
    extract_pawn_double_moves(get_pawn_double_moves(board_state, pawn_single_moves, board_state.turn), legal_moves, board_state.turn);
    extract_moves_with_offset(pawn_single_moves & ~promotion_row, legal_moves, -pawn_shift_map.at(single) * board_state.turn);
}

inline void collect_pawn_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t promotion_row = color_to_promotion_row.at(board_state.turn);
    uint64_t pawn_attacks_left = get_pawn_attacks(board_state, attack_left, board_state.turn);
    uint64_t pawn_attacks_right = get_pawn_attacks(board_state, attack_right, board_state.turn);

    float left_offset = -pawn_shift_map.at(attack_left) * board_state.turn;
    float right_offset = -pawn_shift_map.at(attack_right) * board_state.turn;
    extract_promotion_captures(board_state,
                               pawn_attacks_left & promotion_row & get_enemy_fields(board_state, board_state.turn),
                               legal_moves,
                               left_offset);
    extract_promotion_captures(board_state,
                               pawn_attacks_right & promotion_row & get_enemy_fields(board_state, board_state.turn),
                               legal_moves,
                               right_offset);
    extract_captures_with_offset(board_state,
                                 pawn_attacks_left & (~promotion_row) & get_enemy_fields(board_state, board_state.turn),
                                 legal_moves,
                                 left_offset);
    extract_captures_with_offset(board_state,
                                 pawn_attacks_right & (~promotion_row) & get_enemy_fields(board_state, board_state.turn),
                                 legal_moves,
                                 right_offset);

    uint64_t ep_fields = set_board_1_at_idx(empty_board, board_state.en_passant) & ep_zone;
    extract_ep_captures(ep_fields & pawn_attacks_left, legal_moves, left_offset);
    extract_ep_captures(ep_fields & pawn_attacks_right, legal_moves, right_offset);

    board_state.all_attacks |= (pawn_attacks_left | pawn_attacks_right);
}

inline void collect_knight_moves_and_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t knight_attacks = empty_board;
    for (int knight_idx : scan_board(get_knights(board_state, board_state.turn)))
    {
        uint64_t knight_moves = get_knight_moves(knight_idx);
        knight_attacks |= knight_moves;
        extract_moves_with_explicit_src(knight_moves & get_empty_fields(board_state), legal_moves, knight_idx);
        extract_captures_with_explicit_src(board_state, knight_moves & get_enemy_fields(board_state, board_state.turn), legal_moves, knight_idx);
    }
    board_state.all_attacks |= knight_attacks;
}

inline void collect_king_moves_and_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t king = get_king(board_state, board_state.turn);

    int king_idx = scan_board(king).front();
    uint64_t king_moves = get_king_moves(king_idx);

    extract_moves_with_explicit_src(king_moves & get_empty_fields(board_state), legal_moves, king_idx);
    extract_captures_with_explicit_src(board_state, king_moves & get_enemy_fields(board_state, board_state.turn), legal_moves, king_idx);

    // castling_moves
    const std::array<castling, 2> castling_sides = color_to_castling_indicator.at(board_state.turn);
    if (get_castling_possible(board_state, castling_sides.at(0)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx + 16, 0, 0, 1));
    }
    if (get_castling_possible(board_state, castling_sides.at(1)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx - 16, 0, 0, 2));
    }
    board_state.all_attacks |= king_moves;
}

inline void collect_bishop_moves_and_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t bishop_attacks = empty_board;
    for (int bishop_idx : scan_board(get_bishops(board_state, board_state.turn)))
    {
        uint64_t bishop_moves = get_bishop_moves_and_attacks(bishop_idx, get_all_pieces(board_state));
        bishop_attacks |= bishop_moves;
        extract_moves_with_explicit_src(bishop_moves & get_empty_fields(board_state), legal_moves, bishop_idx);
        extract_captures_with_explicit_src(board_state, bishop_moves & get_enemy_fields(board_state, board_state.turn), legal_moves, bishop_idx);
    }
    board_state.all_attacks |= bishop_attacks;
}

inline void collect_rook_moves_and_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t rook_attacks = empty_board;
    for (int rook_idx : scan_board(get_rooks(board_state, board_state.turn)))
    {
        uint64_t rook_moves = get_rook_moves_and_attacks(rook_idx, get_all_pieces(board_state));
        rook_attacks |= rook_moves;
        extract_moves_with_explicit_src(rook_moves & get_empty_fields(board_state), legal_moves, rook_idx);
        extract_captures_with_explicit_src(board_state, rook_moves & get_enemy_fields(board_state, board_state.turn), legal_moves, rook_idx);
    }
    board_state.all_attacks |= rook_attacks;
}

inline void collect_queen_moves_and_captures(C_BoardState &board_state, MoveList &legal_moves)
{
    uint64_t queen_attacks = empty_board;
    for (int queen_idx : scan_board(get_queen(board_state, board_state.turn)))
    {
        uint64_t queen_moves = get_bishop_moves_and_attacks(queen_idx, get_all_pieces(board_state)) | get_rook_moves_and_attacks(queen_idx, get_all_pieces(board_state));
        queen_attacks |= queen_moves;
        extract_moves_with_explicit_src(queen_moves & get_empty_fields(board_state), legal_moves, queen_idx);
        extract_captures_with_explicit_src(board_state, queen_moves & get_enemy_fields(board_state, board_state.turn), legal_moves, queen_idx);
    }
    board_state.all_attacks |= queen_attacks;
}

inline void collect_legal_moves(C_BoardState &board_state, MoveList &legal_moves)
{
    board_state.all_attacks = empty_board;
    collect_pawn_captures(board_state, legal_moves);
    collect_knight_moves_and_captures(board_state, legal_moves);
    collect_queen_moves_and_captures(board_state, legal_moves);
    collect_rook_moves_and_captures(board_state, legal_moves);
    collect_bishop_moves_and_captures(board_state, legal_moves);
    collect_pawn_moves(board_state, legal_moves);
    collect_king_moves_and_captures(board_state, legal_moves);
}

#endif