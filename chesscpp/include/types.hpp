#ifndef TYPES
#define TYPES

#include <stdint.h>
#include <map>
#include "constants.hpp"

typedef std::array<std::array<int, 8>, 8> Board;
typedef std::vector<std::string> UCIStrings;

const std::map<u_int8_t, char>
    piece_code_to_char = {{0, ' '}, {2, 'q'}, {3, 'b'}, {4, 'n'}, {5, 'r'}};

struct move
{
    uint8_t src;              // 6 b
    uint8_t dest;             // 6 b
    uint8_t capture;          // 3 b (actually less)
    uint8_t promotion;        // 2 b (promotion)
    uint8_t ep_field;         // 6 b
    uint8_t prev_c;           // 4 b
    uint8_t castling;         // 2 b
    uint8_t prev_half_move_c; // 6b
    uint8_t ep;
    // 29b
};

inline move create_move(const u_int8_t src, const uint8_t dest, const uint8_t capture = 0, const uint8_t promotion = 0, const uint8_t cast = 0, const uint8_t ep = 0, uint8_t ep_field = 0)
{
    struct move m;
    m.src = src;             // both
    m.dest = dest;           // both
    m.promotion = promotion; // promotion, both
    m.ep_field = ep_field;   // both but with different use
    m.capture = capture;     // stack/pop
    m.prev_c = 0;            // stack/pop
    m.castling = cast;       // both
    m.prev_half_move_c = 0;  // stack/pop
    m.ep = ep;               // if ep_field == 0 => m.ep = false
    return m;
}

inline move create_empty_move()
{
    return create_move(0, 0);
}

inline bool is_move_empty(move m)
{
    return m.src == m.dest;
}

struct C_BoardState
{
    uint64_t pieces[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t castling_rights = UINT8_C(0x0F);
    uint8_t en_passant = UINT8_C(0x00);
    uint64_t all_attacks = empty_board;
    std::vector<move> move_stack;
    float turn = 1.;
    float moves = 0.;
    float half_moves = 0.;
    bool king_attack = false;
    bool castling_move_illegal = false;
};

typedef std::vector<move>
    MoveList;

struct AlphaBetaState
{
    int max_depth;
    int current_max_depth = 1;
    move bestmove;
    MoveList pvs_best_moves = {};
    std::vector<float> runtimes_at_depth = {};
    std::vector<uint32_t> nodes_at_depth = {};
};

typedef std::vector<MoveList> MoveListStack;

struct C_Session
{
    MoveListStack move_list_stack;
    C_BoardState board_state;
    AlphaBetaState alpha_beta_state;
};

inline void reserve_move_list_stack(MoveListStack &move_list_stack, int depth)
{
    move_list_stack.reserve(depth + 1);
    for (int i = 0; i <= depth; i++)
    {
        MoveList move_list;
        move_list.reserve(20);
        move_list_stack.push_back(move_list);
    }
}

inline void reserve_board_state(C_BoardState &board_state, int depth)
{
    board_state.move_stack.reserve(depth + 1);
}

inline C_Session construct_session(int max_depth)
{
    C_Session session;
    session.alpha_beta_state.max_depth = max_depth;
    session.alpha_beta_state.runtimes_at_depth.resize(max_depth + 1);
    session.alpha_beta_state.nodes_at_depth.resize(max_depth + 1, 0);
    session.alpha_beta_state.pvs_best_moves.resize(max_depth + 1, create_empty_move());

    reserve_move_list_stack(session.move_list_stack, max_depth + max_quiesence_depth);
    reserve_board_state(session.board_state, max_depth + max_quiesence_depth);

    return session;
}

#endif