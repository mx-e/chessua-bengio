#ifndef SESSION
#define SESSION
#include "utils.hpp"
#include "transposition.hpp"

struct C_Session
{
    MoveListStack move_list_stack;
    C_BoardState board_state;
    AlphaBetaState alpha_beta_state;
    HashState hash_state;
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
    board_state.move_stack.reserve(depth + max_quiesence_depth + 1);
}

inline C_Session construct_session(int max_depth)
{
    C_Session session;
    session.alpha_beta_state.max_depth = max_depth;
    session.alpha_beta_state.runtimes_at_depth.resize(max_depth + 1);
    session.alpha_beta_state.nodes_at_depth.resize(max_depth + 1, 0);

    reserve_move_list_stack(session.move_list_stack, max_depth + max_quiesence_depth);
    reserve_board_state(session.board_state, max_depth + max_quiesence_depth);

    construct_hash(session.hash_state);
    return session;
}

#endif