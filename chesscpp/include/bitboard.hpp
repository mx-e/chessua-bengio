#ifndef BB
#define BB
#include <stdint.h>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stack>
#include "bitboard_types.hpp"
#include "bitboard_constants.hpp"
#include "types.hpp"
#include "bitboard_utils.hpp"

struct C_BoardState
{
    uint64_t pieces[8];
    uint8_t castling_rights;
    uint8_t en_passant;
    std::vector<move> move_stack;
    std::vector<int> idx_list;
    float turn;
    float moves;
    float half_moves;
    bool king_attack;
    bool castling_move_illegal;
};

typedef std::vector<move> MoveList;
typedef std::vector<MoveList> MoveListStack;

struct C_Session
{
    MoveListStack move_list_stack;
    C_BoardState board_state;
};

inline void reserve_move_list_stack(MoveListStack &move_list_stack)
{
    move_list_stack.reserve(12);
    for (auto move_list : move_list_stack)
    {
        move_list.reserve(10);
    }
}

inline void reserve_board_state(C_BoardState &board_state)
{
    board_state.move_stack.reserve(10);
    board_state.idx_list.reserve(16);
}

inline C_Session construct_session()
{
    C_Session session{
        .board_state = C_BoardState{
            .castling_rights = UINT8_C(0x0F),
            .en_passant = UINT8_C(0x00),
            .turn = 1.,
            .moves = 0.,
            .half_moves = 0.,
            .king_attack = false,
            .castling_move_illegal = false}};

    reserve_move_list_stack(session.move_list_stack);
    reserve_board_state(session.board_state);    

    return session;
}

// MOVE GENERATION:
// init/alloc
// stack/pop
// collection
// extraction
// expression
// utils

// EXPORTS
// to_uci, etc.

class C_Board
{
public:
    uint64_t pieces[8];

    uint8_t castling_rights = UINT8_C(0x0F);
    uint8_t en_passant = UINT8_C(0x00);

    float turn = 1.;
    float moves = 0.;
    float half_moves = 0.;

    std::vector<move> legal_moves = std::vector<move>();
    std::vector<move> move_stack = std::vector<move>();

    bool king_attack = false;
    bool castling_move_illegal = false;

    inline void set_castling_rights(castling c_type, bool val)
    {
        castling_rights &= (uint8_t)~castling_to_castling_state_mask.at(c_type) & ((uint8_t)val << (int)c_type);
    }

    inline uint64_t get_pieces(const float color, const uint8_t piece_type)
    {
        return pieces[piece_type] & pieces[color_to_BB_index.at(color)];
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

    inline void unset_single_piece(const float color, const uint8_t type, const uint8_t position_idx)
    {
        pieces[type] &= ~(most_sig_bit >> position_idx);
        pieces[color_to_BB_index.at(color)] &= ~(most_sig_bit >> position_idx);
    }

    inline uint64_t get_all_pieces()
    {
        return pieces[b_white] | pieces[b_black];
    }

    inline uint64_t get_empty_fields()
    {
        return ~(get_all_pieces());
    }

    inline uint64_t get_enemy_fields(float color)
    {
        return pieces[color_to_BB_index.at(color * -1.)];
    }

    inline uint64_t get_empty_or_enemy(float color)
    {
        return get_empty_fields() | get_enemy_fields(color);
    }

    inline uint64_t get_pawn_single_moves(const float color)
    {
        return mask_and_shift_by_n(single, get_pawns(color), color) & get_empty_fields();
    }

    inline uint64_t get_pawn_double_moves(uint64_t single_moves, const float color)
    {
        uint64_t half = color == 1. ? white_half : black_half;
        return mask_and_shift_by_n(single, single_moves & center_half, color) & get_empty_fields() & half;
    }

    inline uint64_t get_pawn_attacks(const pawn_shift direction, const float color)
    {
        return mask_and_shift_by_n(direction, get_pawns(color), color);
    }

    inline uint64_t collect_pawn_moves_and_captures();

    inline uint64_t get_knight_moves(const int field_idx)
    {
        return knight_moves[field_idx];
    }

    inline uint64_t collect_knight_moves_and_captures();

    inline uint64_t get_king_moves(const int field_idx)
    {
        return king_moves[field_idx];
    }

    inline bool get_castling_possible(castling c_type)
    {
        uint64_t castling_free = castling_to_castling_free.at(c_type);
        uint8_t state_mask = castling_to_castling_state_mask.at(c_type);
        return (state_mask & castling_rights) && ((get_all_pieces() & castling_free) == 0);
    }

    inline uint64_t collect_king_moves_and_captures();

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

    inline uint64_t collect_bishop_moves_and_captures();

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
    inline uint64_t collect_rook_moves_and_captures();
    inline uint64_t collect_queen_moves_and_captures();

    uint64_t collect_legal_moves();

    void push_move(move m);
    move pop_move();

    inline int get_rating()
    {
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int random_variable = std::rand();
        return (random_variable % 200) - 100;
    }
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
            uint8_t position_idx = col_row_idx_to_position_idx(col, row);
            bitboard.set_single_piece(color, type, position_idx);
        }
    }
    return bitboard;
};

uint8_t get_piece_type_of_field(const C_Board *board, int position_idx);

void extract_moves_with_offset(uint64_t move_board, const std::vector<move> &move_list, int source_offset);

void extract_captures_with_offset(const C_Board *board, uint64_t move_board, const std::vector<move> &move_list, int source_offset);

void extract_promotions(uint64_t move_board, std::vector<move> &move_list);

void extract_promotion_captures(const C_Board *board, uint64_t move_board, const std::vector<move> &move_list);

void extract_moves_with_explicit_src(uint64_t move_board, const std::vector<move> &move_list, int src_idx);

void extract_captures_with_explicit_src(const C_Board &board, uint64_t move_board, const std::vector<move> &move_list, int src_idx);

void execute_move_forward(C_Board &board, move &m);

void execute_move_backward(C_Board &board, move &m);

bool check_castling_move_illegal(C_Board &board, move &m, uint64_t all_attacks);

UCIStrings get_uci_moves(C_Board &board);

inline bool check_move_causes_check(C_Board &board, move &m)
{
    board.push_move(m);
    board.collect_legal_moves();
    bool check = board.king_attack;
    board.pop_move();
    return check;
}

#endif