#ifndef B_TRANSFORM
#define B_TRANSFORM

#include "bitboard_constants.hpp"
#include "bitboard_types.hpp"
#include "bitboard_expressions.hpp"
#include "bitboard_utils.hpp"
#include "utils.hpp"

inline void set_castling_rights(C_BoardState &board, castling c_type)
{
    int address_bit = (int)val << 1;
    board.castling_rights |= address_bit;
}

inline void unset_castling_rights(C_BoardState &board, castling c_type)
{
    board.castling_rights &= ~castling_to_castling_state_mask.at(c_type);
}

inline bool get_castling_possible(C_BoardState &board, castling c_type)
{
    uint64_t castling_free = castling_to_castling_free.at(c_type);
    uint8_t state_mask = castling_to_castling_state_mask.at(c_type);
    return (state_mask & castling_rights) && ((get_all_pieces(board) & castling_free) == 0);
}

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

inline void execute_move_forward(C_Board &board, const move &m)
{
    uint8_t piece_type = get_piece_type_of_field(board, m.src);
    unset_single_piece(board, board.turn, piece_type, m.src);
    if (m.capture)
    {
        unset_single_piece(board, board.turn * -1, m.capture, m.dest);
    }
    if (m.flag)
    {
        piece_type = m.flag;
    }
    set_single_piece(board, board.turn, piece_type, m.dest);
    if (m.castling)
    {
        if (m.castling == 1)
        {
            board.unset_single_piece(board, board.turn, pRook, 56);
            board.set_single_piece(board, board.turn, pRook, 40);
        }
        else if (m.castling == 2)
        {
            unset_single_piece(board, board.turn, pRook, 0);
            set_single_piece(board, board.turn, pRook, 24);
        }
    }
    if (m.ep)
    {
        board.unset_single_piece(board.turn, m.capture, m.dest - (int)board.turn);
    }
}

inline void execute_move_backward(C_Board &board, const move &m)
{
    uint8_t piece_type = get_piece_type_of_field(board, m.dest);
    unset_single_piece(board, board.turn * -1, piece_type, m.dest);
    if (m.capture)
    {
        set_single_piece(board, board.turn, m.capture, m.dest);
    }
    if (m.flag)
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

inline void update_castling_rights(C_Board &board)
{
    uint64_t king_and_rooks = board.get_rooks(board.turn) | board.get_king(board.turn);
    std::array<uint64_t, 2> castling_masks = color_to_castling_mask.at(board.turn);
    std::array<castling, 2> castling_indicator = color_to_castling_indicator.at(board.turn);

    bool ks_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[0]));
    bool qs_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[1]));

    board.set_castling_rights(castling_indicator[0], ks_castling_rights);
    board.set_castling_rights(castling_indicator[1], qs_castling_rights);
}

inline bool check_castling_move_illegal(C_Board &board, const move &m, const uint64_t all_attacks)
{
    std::array<uint64_t, 2> no_attack_masks = color_to_castling_no_attack.at(board.turn * -1);
    bool match_kingside = (bool)(all_attacks & no_attack_masks[0]);
    bool match_queenside = (bool)(all_attacks & no_attack_masks[1]);
    return (m.castling == 1 && match_kingside) || (m.castling == 2 && match_queenside);
}

inline void C_Board::push_move(C_BoardState &board, move m)
{
    execute_move_forward(board, m);

    m.prev_c = castling_rights;
    update_castling_rights(board);

    uint8_t ep_field = m.ep_field;
    m.ep_field = en_passant;
    en_passant = ep_field;

    // set counters
    uint8_t piece_type = get_piece_type_of_field(board, m.dest);
    m.prev_half_move_c = (uint8_t)half_moves;
    half_moves += 1.;
    half_moves *= ((!(piece_type == pPawn) && !(m.capture)));
    moves += (turn == -1.) * 1.;

    turn *= -1;

    // uint64_t all_new_attacks = collect_legal_moves(board);
    // king_attack = (bool)(all_new_attacks & get_king(-1. * turn));
    // castling_move_illegal = check_castling_move_illegal(board, m, all_new_attacks);

    move_stack.push_back(m);
}

inline move C_Board::pop_move(C_BoardState &board)
{
    if (move_stack.empty())
    {
        return create_move(0, 0);
    }
    move m = move_stack.back();

    execute_move_backward(board, m);

    castling_rights = m.prev_c;

    uint8_t ep_field = m.ep_field;
    m.ep_field = en_passant;
    en_passant = ep_field;

    // set_counters
    half_moves = m.prev_half_move_c;
    moves -= (turn == 1.) * 1.;

    turn *= -1;
    king_attack = false;
    castling_move_illegal = false;
    move_stack.pop_back();
    legal_moves = std::vector<move>();
    return m;
}

inline bool check_move_causes_check(C_Board &board, move &m)
{
    push_move(board, m);
    // collect_legal_moves(board);
    bool check = board.king_attack;
    board.pop_move(board);
    return check;
}

#endif