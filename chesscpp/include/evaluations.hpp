#ifndef EVALUATIONS
#define EVALUATIONS

#include "utils.hpp"
#include "expressions.hpp"
#include "constants.hpp"
#include "evaluation_constants.hpp"

inline int rooks_on_open_file(C_BoardState &board_state, float color)
{
    int n_rooks = 0;
    uint64_t all_pieces = get_all_pieces(board_state);
    for (uint64_t col : cols)
    {
        if ((col & get_rooks(board_state, color)) && ((get_rooks(board_state, color) & col) == (all_pieces & col)))
            n_rooks++;
    }
    return n_rooks;
}

inline bool has_bishop_pair(C_BoardState &board_state, float color)
{
    uint64_t bishops = get_bishops(board_state, color);
    return ((bishops & black_squares) != empty_board) && ((bishops & white_squares) != empty_board);
}

inline int n_pawns_shielding_king(C_BoardState &board_state, float color)
{
    int king_field = forward_scan(get_king(board_state, color));
    return bb_pop_count(king_shield[color_to_king_shield_idx.at(color)][king_field] & get_pawns(board_state, color));
}

inline int passed_pawns(const C_BoardState &board, float color)
{
    int passed = 0;
    uint64_t pawns = get_pawns(board, color);

    while (pawns != empty_board)
    {
        int field = forward_scan(pawns);
        if ((get_pawns(board, -color) & passed_pawn_mask[color_to_pawn_mask_idx.at(color)][field]) == 0)
            passed++;
        pawns &= ~cols[(int)field / 8];
    }
    return passed;
}

inline int doubled_pawns(const C_BoardState &board, float color)
{
    int doubled = 0;
    for (uint64_t col : cols)
    {
        int pawns_on_file = bb_pop_count(get_pawns(board, color) & col);
        if (pawns_on_file > 1)
            doubled += (pawns_on_file - 1);
    }
    return doubled;
}

inline int isolated_pawns(const C_BoardState &board, float color)
{
    int isolated = 0;
    uint64_t pawns = get_pawns(board, color);
    for (int col_idx = 0; col_idx < 8; col_idx++)
    {
        uint64_t mask = cols[col_idx];
        uint64_t adj_mask = isolated_pawn_mask[col_idx];
        if ((pawns & mask) && !(pawns & adj_mask))
            isolated++;
    }
    return isolated;
}

inline int get_pawn_move_count(C_BoardState &board_state, float color)
{
    uint64_t pawn_single_moves = get_pawn_single_moves(board_state, color);
    uint64_t pawn_double_moves = get_pawn_double_moves(board_state, pawn_single_moves, color);
    uint64_t pawn_attacks_left = get_pawn_attacks(board_state, attack_left, color);
    uint64_t pawn_attacks_right = get_pawn_attacks(board_state, attack_right, color);
    return bb_pop_count(pawn_single_moves | pawn_double_moves | ((pawn_attacks_left | pawn_attacks_right) & get_enemy_fields(board_state, color)));
}

inline int get_king_move_count(C_BoardState &board_state, float color)
{
    uint64_t king = get_king(board_state, color);
    int king_idx = king ? forward_scan(king) : 0;
    return king ? bb_pop_count(get_king_moves(king_idx) & get_enemy_fields(board_state, color)) : 0;
}

inline int get_knight_move_count(C_BoardState &board_state, float color)
{
    int n_knight_attacks = 0;
    uint64_t knights = get_knights(board_state, color);
    while (knights)
    {
        int field = forward_scan(knights);
        n_knight_attacks += bb_pop_count(get_knight_moves(field) & get_enemy_fields(board_state, color));
        knights &= knights - 1;
    }
    return n_knight_attacks;
}

inline int get_rook_move_count(C_BoardState &board_state, float color)
{
    int n_rook_attacks = 0;
    uint64_t rooks = get_rooks(board_state, color);
    while (rooks)
    {
        int field = forward_scan(rooks);
        n_rook_attacks += bb_pop_count(get_rook_moves_and_attacks(field, get_all_pieces(board_state)) & get_enemy_fields(board_state, color));
        rooks &= rooks - 1;
    }
    return n_rook_attacks;
}

inline int get_bishop_move_count(C_BoardState &board_state, float color)
{
    int n_bishop_attacks = 0;
    uint64_t bishops = get_bishops(board_state, color);
    while (bishops)
    {
        int field = forward_scan(bishops);
        n_bishop_attacks += bb_pop_count(get_bishop_moves_and_attacks(field, get_all_pieces(board_state)) & get_enemy_fields(board_state, color));
        bishops &= bishops - 1;
    }
    return n_bishop_attacks;
}

inline int get_queen_move_count(C_BoardState &board_state, float color)
{
    int n_queen_attacks = 0;
    uint64_t queens = get_queen(board_state, color);
    while (queens)
    {
        int field = forward_scan(queens);
        n_queen_attacks += bb_pop_count(
            (get_rook_moves_and_attacks(field, get_all_pieces(board_state)) | get_bishop_moves_and_attacks(field, get_all_pieces(board_state))) & get_enemy_fields(board_state, color));
        queens &= queens - 1;
    }
    return n_queen_attacks;
}

inline void evaluate_material(C_BoardState &board_state, Scores &score)
{
    uint64_t w_pawns = get_pawns(board_state, White);
    uint64_t b_pawns = get_pawns(board_state, Black);
    score[OPENING] += material_values[OPENING][pPawn] * bb_pop_count(w_pawns);
    score[OPENING] += material_values[OPENING][pPawn] * -bb_pop_count(b_pawns);

    score[ENDGAME] += material_values[ENDGAME][pPawn] * bb_pop_count(w_pawns);
    score[ENDGAME] += material_values[ENDGAME][pPawn] * -bb_pop_count(b_pawns);

    uint64_t w_knights = get_knights(board_state, White);
    uint64_t b_knights = get_knights(board_state, Black);
    score[OPENING] += material_values[OPENING][pKnight] * bb_pop_count(w_knights);
    score[OPENING] += material_values[OPENING][pKnight] * -bb_pop_count(b_knights);

    score[ENDGAME] += material_values[ENDGAME][pKnight] * bb_pop_count(w_knights);
    score[ENDGAME] += material_values[ENDGAME][pKnight] * -bb_pop_count(b_knights);

    uint64_t w_bishops = get_bishops(board_state, White);
    uint64_t b_bishops = get_bishops(board_state, Black);
    score[OPENING] += material_values[OPENING][pBishop] * bb_pop_count(w_bishops);
    score[OPENING] += material_values[OPENING][pBishop] * -bb_pop_count(b_bishops);

    score[ENDGAME] += material_values[ENDGAME][pBishop] * bb_pop_count(w_bishops);
    score[ENDGAME] += material_values[ENDGAME][pBishop] * -bb_pop_count(b_bishops);

    uint64_t w_rook = get_rooks(board_state, White);
    uint64_t b_rook = get_rooks(board_state, Black);
    score[OPENING] += material_values[OPENING][pRook] * bb_pop_count(w_rook);
    score[OPENING] += material_values[OPENING][pRook] * -bb_pop_count(b_rook);

    score[ENDGAME] += material_values[ENDGAME][pRook] * bb_pop_count(w_rook);
    score[ENDGAME] += material_values[ENDGAME][pRook] * -bb_pop_count(b_rook);

    uint64_t w_queen = get_queen(board_state, White);
    uint64_t b_queen = get_queen(board_state, Black);
    score[OPENING] += material_values[OPENING][pQueen] * bb_pop_count(w_queen);
    score[OPENING] += material_values[OPENING][pQueen] * -bb_pop_count(b_queen);

    score[ENDGAME] += material_values[ENDGAME][pQueen] * bb_pop_count(w_queen);
    score[ENDGAME] += material_values[ENDGAME][pQueen] * -bb_pop_count(b_queen);
}

inline void evaluate_mobility(C_BoardState &board_state, Scores &score)
{
    int w_pawn_move_count = get_pawn_move_count(board_state, White);
    int b_pawn_move_count = get_pawn_move_count(board_state, Black);
    score[OPENING] += w_pawn_move_count * mobility_bonus[OPENING][b_pawns];
    score[OPENING] -= b_pawn_move_count * mobility_bonus[OPENING][b_pawns];

    score[ENDGAME] += w_pawn_move_count * mobility_bonus[ENDGAME][b_pawns];
    score[ENDGAME] -= b_pawn_move_count * mobility_bonus[ENDGAME][b_pawns];

    int w_king_attack_count = get_king_move_count(board_state, White);
    int b_king_attack_count = get_king_move_count(board_state, Black);
    score[OPENING] += w_king_attack_count * mobility_bonus[OPENING][b_kings];
    score[OPENING] -= b_king_attack_count * mobility_bonus[OPENING][b_kings];

    score[ENDGAME] += w_king_attack_count * mobility_bonus[ENDGAME][b_kings];
    score[ENDGAME] -= b_king_attack_count * mobility_bonus[ENDGAME][b_kings];

    int w_queen_attack_count = get_queen_move_count(board_state, White);
    int b_queen_attack_count = get_queen_move_count(board_state, Black);
    score[OPENING] += w_queen_attack_count * mobility_bonus[OPENING][b_queens];
    score[OPENING] -= b_queen_attack_count * mobility_bonus[OPENING][b_queens];

    score[ENDGAME] += w_queen_attack_count * mobility_bonus[ENDGAME][b_queens];
    score[ENDGAME] -= b_queen_attack_count * mobility_bonus[ENDGAME][b_queens];

    int w_rook_attack_count = get_rook_move_count(board_state, White);
    int b_rook_attack_count = get_rook_move_count(board_state, Black);
    score[OPENING] += w_rook_attack_count * mobility_bonus[OPENING][b_rooks];
    score[OPENING] -= b_rook_attack_count * mobility_bonus[OPENING][b_rooks];

    score[ENDGAME] += w_rook_attack_count * mobility_bonus[ENDGAME][b_rooks];
    score[ENDGAME] -= b_rook_attack_count * mobility_bonus[ENDGAME][b_rooks];

    int w_bishop_attack_count = get_bishop_move_count(board_state, White);
    int b_bishop_attack_count = get_bishop_move_count(board_state, Black);
    score[OPENING] += w_bishop_attack_count * mobility_bonus[OPENING][b_bishops];
    score[OPENING] -= b_bishop_attack_count * mobility_bonus[OPENING][b_bishops];

    score[ENDGAME] += w_bishop_attack_count * mobility_bonus[ENDGAME][b_bishops];
    score[ENDGAME] -= b_bishop_attack_count * mobility_bonus[ENDGAME][b_bishops];

    int w_knight_attack_count = get_knight_move_count(board_state, White);
    int b_knight_attack_count = get_knight_move_count(board_state, Black);
    score[OPENING] += w_knight_attack_count * mobility_bonus[OPENING][b_knights];
    score[OPENING] -= b_knight_attack_count * mobility_bonus[OPENING][b_knights];

    score[ENDGAME] += w_knight_attack_count * mobility_bonus[ENDGAME][b_knights];
    score[ENDGAME] -= b_knight_attack_count * mobility_bonus[ENDGAME][b_knights];
}

inline void evaluate_rooks_on_open_file(C_BoardState &board, Scores &score)
{
    int rook_on_open_file_diff = rooks_on_open_file(board, White) - rooks_on_open_file(board, Black);

    score[OPENING] += rook_open_file_bonus[OPENING] * rook_on_open_file_diff;
    score[ENDGAME] += rook_open_file_bonus[ENDGAME] * rook_on_open_file_diff;
}

inline void evaluate_has_bishop_pair(C_BoardState &board, Scores &score)
{
    bool has_b_pair_w = has_bishop_pair(board, White);
    bool has_b_pair_b = has_bishop_pair(board, Black);
    score[OPENING] += has_b_pair_w * bishop_pair_bonus[OPENING];
    score[OPENING] -= has_b_pair_b * bishop_pair_bonus[OPENING];

    score[ENDGAME] += has_b_pair_w * bishop_pair_bonus[ENDGAME];
    score[ENDGAME] -= has_b_pair_b * bishop_pair_bonus[ENDGAME];
}

inline void evaluate_king_shield(C_BoardState &board, Scores &score)
{
    int king_pawn_shield_w = n_pawns_shielding_king(board, White);
    int king_pawn_shield_b = n_pawns_shielding_king(board, Black);
    score[OPENING] += king_pawn_shield_w * king_pawn_shield_bonus[OPENING];
    score[OPENING] -= king_pawn_shield_b * king_pawn_shield_bonus[OPENING];

    score[ENDGAME] += king_pawn_shield_w * king_pawn_shield_bonus[ENDGAME];
    score[ENDGAME] -= king_pawn_shield_b * king_pawn_shield_bonus[ENDGAME];
}

inline void evaluate_pawn_structure(const C_BoardState &board, Scores &scores)
{
    int passed_pawn_diff = passed_pawns(board, White) - passed_pawns(board, Black);
    scores[OPENING] += passed_pawn_bonus[OPENING] * passed_pawn_diff;
    scores[ENDGAME] += passed_pawn_bonus[ENDGAME] * passed_pawn_diff;

    int doubled_pawns_diff = doubled_pawns(board, White) - doubled_pawns(board, Black);
    scores[OPENING] += doubled_pawn_penalty[OPENING] * doubled_pawns_diff;
    scores[ENDGAME] += doubled_pawn_penalty[ENDGAME] * doubled_pawns_diff;

    int isolated_pawn_diff = isolated_pawns(board, White) - isolated_pawns(board, Black);
    scores[OPENING] += isolated_pawn_penalty[OPENING] * isolated_pawn_diff;
    scores[ENDGAME] += isolated_pawn_penalty[ENDGAME] * isolated_pawn_diff;
}

inline void evaluate_ps_tables(const C_BoardState &board, Scores &scores)
{
    scores[OPENING] += board.ps_score[color_to_pstable_idx.at(White)][OPENING];
    scores[OPENING] -= board.ps_score[color_to_pstable_idx.at(Black)][OPENING];

    scores[ENDGAME] += board.ps_score[color_to_pstable_idx.at(White)][ENDGAME];
    scores[ENDGAME] -= board.ps_score[color_to_pstable_idx.at(Black)][ENDGAME];
}

inline int get_phase(C_BoardState &board)
{
    int phase_score = initial_phase_weights;
    for (auto piece_type : {b_rooks, b_bishops, b_queens, b_knights})
    {
        phase_score -= bb_pop_count(board.pieces[piece_type]) * phase_weights[piece_type];
    }
    return phase_score;
}

inline float interpolate_scores(C_BoardState &board, Scores &score)
{
    int phase_score = get_phase(board);
    return (score[OPENING] * (initial_phase_weights - phase_score) + score[ENDGAME] * phase_score) / initial_phase_weights;
}

inline float evaluate(C_BoardState &board_state)
{
    Scores score = {0., 0.};

    evaluate_material(board_state, score);
    evaluate_pawn_structure(board_state, score);
    evaluate_rooks_on_open_file(board_state, score);
    evaluate_has_bishop_pair(board_state, score);
    evaluate_king_shield(board_state, score);
    evaluate_mobility(board_state, score);
    evaluate_ps_tables(board_state, score);

    return board_state.turn * interpolate_scores(board_state, score);
}

#endif