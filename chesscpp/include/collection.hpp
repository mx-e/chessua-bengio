#ifndef COLLECTION
#define COLLECTION

#include "types.hpp"

inline uint64_t collect_pawn_moves_and_captures(C_BoardState board_state)
{
    uint64_t promotion_row = turn == 1. ? row_8 : row_1;

    uint64_t pawn_single_moves = get_pawn_single_moves(turn);

    extract_moves_with_offset(pawn_single_moves & ~promotion_row, legal_moves, -pawn_shift_map.at(single) * turn);
    extract_promotions(pawn_single_moves & promotion_row, legal_moves, -pawn_shift_map.at(single) * turn);

    extract_pawn_double_moves(get_pawn_double_moves(pawn_single_moves, turn), legal_moves, turn);

    uint64_t pawn_attacks_left = get_pawn_attacks(attack_left, turn);
    uint64_t pawn_attacks_right = get_pawn_attacks(attack_right, turn);

    float left_offset = -pawn_shift_map.at(attack_left) * turn;
    float right_offset = -pawn_shift_map.at(attack_right) * turn;
    extract_captures_with_offset(this,
                                 pawn_attacks_left & (~promotion_row) & get_enemy_fields(turn),
                                 legal_moves,
                                 left_offset);
    extract_captures_with_offset(this,
                                 pawn_attacks_right & (~promotion_row) & get_enemy_fields(turn),
                                 legal_moves,
                                 right_offset);
    extract_promotion_captures(this,
                               pawn_attacks_left & promotion_row & get_enemy_fields(turn),
                               legal_moves,
                               left_offset);
    extract_promotion_captures(this,
                               pawn_attacks_right & promotion_row & get_enemy_fields(turn),
                               legal_moves,
                               right_offset);

    uint64_t ep_fields = set_board_1_at_idx(empty_board, en_passant) & ep_zone;
    extract_ep_captures(*this, ep_fields & pawn_attacks_left, legal_moves, left_offset);
    extract_ep_captures(*this, ep_fields & pawn_attacks_right, legal_moves, right_offset);

    return pawn_attacks_left | pawn_attacks_right;
}

uint64_t C_Board::collect_knight_moves_and_captures()
{
    uint64_t knight_attacks = empty_board;
    for (int knight_idx : scan_board(get_knights(turn)))
    {
        uint64_t knight_moves = get_knight_moves(knight_idx);
        knight_attacks |= knight_moves;
        extract_moves_with_explicit_src(knight_moves & get_empty_fields(), legal_moves, knight_idx);
        extract_captures_with_explicit_src(this, knight_moves & get_enemy_fields(turn), legal_moves, knight_idx);
    }
    return knight_attacks;
}

inline uint64_t C_Board::collect_king_moves_and_captures()
{
    uint64_t king = get_king(turn);
    if (!king)
        return 0;
    int king_idx = scan_board(king).front();
    uint64_t king_moves = get_king_moves(king_idx);
    extract_moves_with_explicit_src(king_moves & get_empty_fields(), legal_moves, king_idx);
    extract_captures_with_explicit_src(this, king_moves & get_enemy_fields(turn), legal_moves, king_idx);

    // castling_moves
    const std::array<castling, 2> castling_sides = color_to_castling_indicator.at(turn);
    if (get_castling_possible(castling_sides.at(0)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx + 16, 0, 0, 1));
    }
    if (get_castling_possible(castling_sides.at(1)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx - 16, 0, 0, 2));
    }
    return king_moves;
}

uint64_t C_Board::collect_bishop_moves_and_captures()
{
    uint64_t bishop_attacks = empty_board;
    for (int bishop_idx : scan_board(get_bishops(turn)))
    {
        uint64_t bishop_moves = get_bishop_moves_and_attacks(bishop_idx, get_all_pieces());
        bishop_attacks |= bishop_moves;
        extract_moves_with_explicit_src(bishop_moves & get_empty_fields(), legal_moves, bishop_idx);
        extract_captures_with_explicit_src(this, bishop_moves & get_enemy_fields(turn), legal_moves, bishop_idx);
    }
    return bishop_attacks;
}

uint64_t C_Board::collect_rook_moves_and_captures()
{
    uint64_t rook_attacks = empty_board;
    for (int rook_idx : scan_board(get_rooks(turn)))
    {
        uint64_t rook_moves = get_rook_moves_and_attacks(rook_idx, get_all_pieces());
        rook_attacks |= rook_moves;
        extract_moves_with_explicit_src(rook_moves & get_empty_fields(), legal_moves, rook_idx);
        extract_captures_with_explicit_src(this, rook_moves & get_enemy_fields(turn), legal_moves, rook_idx);
    }
    return rook_attacks;
}

uint64_t C_Board::collect_queen_moves_and_captures()
{
    uint64_t queen_attacks = empty_board;
    for (int queen_idx : scan_board(get_queen(turn)))
    {
        uint64_t queen_moves = get_bishop_moves_and_attacks(queen_idx, get_all_pieces()) | get_rook_moves_and_attacks(queen_idx, get_all_pieces());
        queen_attacks |= queen_moves;
        extract_moves_with_explicit_src(queen_moves & get_empty_fields(), legal_moves, queen_idx);
        extract_captures_with_explicit_src(this, queen_moves & get_enemy_fields(turn), legal_moves, queen_idx);
    }
    return queen_attacks;
}

uint64_t C_Board::collect_legal_moves()
{
    uint64_t all_attacks = empty_board;
    legal_moves = std::vector<move>();
    legal_moves.reserve(32);
    all_attacks |= collect_pawn_moves_and_captures();
    all_attacks |= collect_knight_moves_and_captures();
    all_attacks |= collect_king_moves_and_captures();
    all_attacks |= collect_bishop_moves_and_captures();
    all_attacks |= collect_rook_moves_and_captures();
    all_attacks |= collect_queen_moves_and_captures();
    return all_attacks;
}


#endif