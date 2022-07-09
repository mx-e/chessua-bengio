#ifndef EVALUATIONS
#define EVALUATIONS

#include "utils.hpp"
#include "expressions.hpp"
#include "pawn_structure.hpp"
#include "constants.hpp"

const int material_values[2][7] = {
    [OPENING] = {
        [pPawn] = 100,
        [pRook] = 500,
        [pKnight] = 320,
        [pBishop] = 330,
        [pQueen] = 900,
        [pKing] = 0},
    [ENDGAME] = {[pPawn] = 140, [pRook] = 500, [pKnight] = 300, [pBishop] = 300, [pQueen] = 900, [pKing] = 0}};

const int phase_weights[7] = {
    [b_pawns] = 0,
    [b_rooks] = 2,
    [b_knights] = 1,
    [b_bishops] = 1,
    [b_queens] = 4,
    [b_kings] = 0};

const int initial_phase_weights =
    16 * phase_weights[b_pawns] +
    4 * phase_weights[b_rooks] +
    4 * phase_weights[b_knights] +
    4 * phase_weights[b_bishops] +
    2 * phase_weights[b_queens] +
    2 * phase_weights[b_kings];

const int rook_open_file_bonus[2] = {[OPENING] = 20, [ENDGAME] = 40};
const int bishop_pair_bonus[2] = {[OPENING] = 45, [ENDGAME] = 55};
const int king_pawn_shield_bonus[2] = {[OPENING] = 10, [ENDGAME] = 0};

const int mobility_bonus[2][7] = {
    [OPENING] = {
        [pPawn] = 0,
        [pRook] = 0,
        [pKnight] = 4,
        [pBishop] = 3,
        [pQueen] = 0,
        [pKing] = 0},
    [ENDGAME] = {[pPawn] = 1, [pRook] = 1, [pKnight] = 6, [pBishop] = 2, [pQueen] = 1, [pKing] = 1}};

const std::map<float, int> color_to_king_shield_idx = {{1., 0}, {-1, 1}};
const uint64_t king_shield[1][64] =
    {
        {
            0x6060000000000000LL,
            0x3030000000000000LL,
            0x1818000000000000LL,
            0xc0c000000000000LL,
            0x606000000000000LL,
            0x303000000000000LL,
            0x101000000000000LL,
            0x0LL,
            0x6060600000000000LL,
            0x3030300000000000LL,
            0x1818180000000000LL,
            0xc0c0c0000000000LL,
            0x606060000000000LL,
            0x303030000000000LL,
            0x101010000000000LL,
            0x0LL,
            0x60606000000000LL,
            0x30303000000000LL,
            0x18181800000000LL,
            0xc0c0c00000000LL,
            0x6060600000000LL,
            0x3030300000000LL,
            0x1010100000000LL,
            0x0LL,
            0x606060000000LL,
            0x303030000000LL,
            0x181818000000LL,
            0xc0c0c000000LL,
            0x60606000000LL,
            0x30303000000LL,
            0x10101000000LL,
            0x0LL,
            0x6060600000LL,
            0x3030300000LL,
            0x1818180000LL,
            0xc0c0c0000LL,
            0x606060000LL,
            0x303030000LL,
            0x101010000LL,
            0x0LL,
            0x60606000LL,
            0x30303000LL,
            0x18181800LL,
            0xc0c0c00LL,
            0x6060600LL,
            0x3030300LL,
            0x1010100LL,
            0x0LL,
            0x606060LL,
            0x303030LL,
            0x181818LL,
            0xc0c0cLL,
            0x60606LL,
            0x30303LL,
            0x10101LL,
            0x0LL,
            0x6060LL,
            0x3030LL,
            0x1818LL,
            0xc0cLL,
            0x606LL,
            0x303LL,
            0x101LL,
            0x0LL,
        },
        {
            0x0LL,
            0x8080000000000000LL,
            0xc0c0000000000000LL,
            0x6060000000000000LL,
            0x3030000000000000LL,
            0x1818000000000000LL,
            0xc0c000000000000LL,
            0x606000000000000LL,
            0x0LL,
            0x8080800000000000LL,
            0xc0c0c00000000000LL,
            0x6060600000000000LL,
            0x3030300000000000LL,
            0x1818180000000000LL,
            0xc0c0c0000000000LL,
            0x606060000000000LL,
            0x0LL,
            0x80808000000000LL,
            0xc0c0c000000000LL,
            0x60606000000000LL,
            0x30303000000000LL,
            0x18181800000000LL,
            0xc0c0c00000000LL,
            0x6060600000000LL,
            0x0LL,
            0x808080000000LL,
            0xc0c0c0000000LL,
            0x606060000000LL,
            0x303030000000LL,
            0x181818000000LL,
            0xc0c0c000000LL,
            0x60606000000LL,
            0x0LL,
            0x8080800000LL,
            0xc0c0c00000LL,
            0x6060600000LL,
            0x3030300000LL,
            0x1818180000LL,
            0xc0c0c0000LL,
            0x606060000LL,
            0x0LL,
            0x80808000LL,
            0xc0c0c000LL,
            0x60606000LL,
            0x30303000LL,
            0x18181800LL,
            0xc0c0c00LL,
            0x6060600LL,
            0x0LL,
            0x808080LL,
            0xc0c0c0LL,
            0x606060LL,
            0x303030LL,
            0x181818LL,
            0xc0c0cLL,
            0x60606LL,
            0x0LL,
            0x8080LL,
            0xc0c0LL,
            0x6060LL,
            0x3030LL,
            0x1818LL,
            0xc0cLL,
            0x606LL,
        }};

void evaluate_material(C_BoardState &board_state, Scores &score)
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
    return bb_pop_count(king_shield[color_to_king_shield_idx.at(color)][king_field] && get_pawns(board_state, White));
}

inline int pawn_move_count(C_BoardState &board_state, float color)
{
    uint64_t pawn_single_moves = get_pawn_single_moves(board_state, color);
    uint64_t pawn_double_moves = get_pawn_double_moves(board_state, pawn_single_moves, color);
    uint64_t pawn_attacks_left = get_pawn_attacks(board_state, attack_left, color);
    uint64_t pawn_attacks_right = get_pawn_attacks(board_state, attack_right, color);
    return bb_pop_count(pawn_single_moves | pawn_double_moves | pawn_attacks_left | pawn_attacks_right);
}

inline int king_move_count(C_BoardState &board_state, float color)
{
    uint64_t king = get_king(board_state, color);
    return king ? bb_pop_count(get_king_moves(king) & get_enemy_fields(board_state, color)) : 0;
}

inline int get_knight_moves_count(C_BoardState &board_state, float color)
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

inline int get_rook_moves_count(C_BoardState &board_state, float color)
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

inline int get_bishop_moves_count(C_BoardState &board_state, float color)
{
    int n_bishop_attacks = 0;
    uint64_t bishops = get_bishops(board_state, color);
    while (bishops)
    {
        int field = forward_scan(bishops);
        n_bishop_attacks += bb_pop_count(get_rook_moves_and_attacks(field, get_all_pieces(board_state)) & get_enemy_fields(board_state, color));
        bishops &= bishops - 1;
    }
    return n_bishop_attacks;
}

inline int get_queen_moves_count(C_BoardState &board_state, float color)
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

inline int evaluate_mobility_score(C_BoardState &board_state, Scores &score)
{
    int score = 0;
    int w_pawn_move_count = pawn_move_count(board_state, White);
    int b_pawn_move_count = pawn_move_count(board_state, Black);
    score[OPENING] += w_pawn_move_count * mobility_bonus[OPENING][b_pawns];
    score[OPENING] -= b_pawn_move_count * mobility_bonus[OPENING][b_pawns];

    score[ENDGAME] += w_pawn_move_count * mobility_bonus[ENDGAME][b_pawns];
    score[ENDGAME] -= b_pawn_move_count * mobility_bonus[ENDGAME][b_pawns];

    int w_king_attack_count = king_move_count(board_state, White);
    int b_king_attack_count = king_move_count(board_state, Black);
    score[OPENING] += w_king_attack_count * mobility_bonus[OPENING][b_kings];
    score[OPENING] -= b_king_attack_count * mobility_bonus[OPENING][b_kings];

    score[ENDGAME] += w_king_attack_count * mobility_bonus[ENDGAME][b_kings];
    score[ENDGAME] -= b_king_attack_count * mobility_bonus[ENDGAME][b_kings];

    int w_queen_attack_count = get_queen_moves_count(board_state, White);
    int b_queen_attack_count = get_queen_moves_count(board_state, Black);
    score[OPENING] += w_queen_attack_count * mobility_bonus[OPENING][b_queens];
    score[OPENING] -= b_queen_attack_count * mobility_bonus[OPENING][b_queens];

    score[ENDGAME] += w_queen_attack_count * mobility_bonus[ENDGAME][b_queens];
    score[ENDGAME] -= b_queen_attack_count * mobility_bonus[ENDGAME][b_queens];

    int w_rook_attack_count = get_rook_moves_count(board_state, White);
    int b_rook_attack_count = get_rook_moves_count(board_state, Black);
    score[OPENING] += w_rook_attack_count * mobility_bonus[OPENING][b_rooks];
    score[OPENING] -= b_rook_attack_count * mobility_bonus[OPENING][b_rooks];

    score[ENDGAME] += w_rook_attack_count * mobility_bonus[ENDGAME][b_rooks];
    score[ENDGAME] -= b_rook_attack_count * mobility_bonus[ENDGAME][b_rooks];

    int w_bishop_attack_count = get_bishop_moves_count(board_state, White);
    int b_bishop_attack_count = get_bishop_moves_count(board_state, Black);
    score[OPENING] += w_bishop_attack_count * mobility_bonus[OPENING][b_bishops];
    score[OPENING] -= b_bishop_attack_count * mobility_bonus[OPENING][b_bishops];

    score[ENDGAME] += w_bishop_attack_count * mobility_bonus[ENDGAME][b_bishops];
    score[ENDGAME] -= b_bishop_attack_count * mobility_bonus[ENDGAME][b_bishops];

    int w_knight_attack_count = get_knight_moves_count(board_state, White);
    int b_knight_attack_count = get_knight_moves_count(board_state, Black);
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

inline float interpolate_scores(C_BoardState &board, Scores &score)
{
    int phase_score = initial_phase_weights;
    for (auto piece_type : {b_rooks, b_bishops, b_queens, b_knights})
    {
        phase_score -= bb_pop_count(board.pieces[piece_type]) * phase_weights[piece_type];
        phase_score -= bb_pop_count(board.pieces[piece_type]) * phase_weights[piece_type];
    }

    return (score[OPENING] * (initial_phase_weights - phase_score) + score[ENDGAME] * phase_score) / initial_phase_weights;
}

inline float evaluate(C_BoardState &board_state)
{
    Scores score = {0.};

    evaluate_material(board_state, score);
    evaluate_pawn_structure(board_state, score);
    evaluate_rooks_on_open_file(board_state, score);
    evaluate_has_bishop_pair(board_state, score);
    evaluate_king_shield(board_state, score);
    evaluate_mobility_score(board_state, score);

    return interpolate_scores(board_state, score);
}

#endif