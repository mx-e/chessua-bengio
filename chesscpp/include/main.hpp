#ifndef MAIN
#define MAIN

#include "transforms.hpp"
#include "collections.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include "search.hpp"
#include "session.hpp"
#include "conversions.hpp"
#include <chrono>

inline UCIStrings get_uci_moves(C_BoardState &board_state)
{
    MoveList moves;
    moves.reserve(32);
    collect_legal_moves(board_state, moves);

    UCIStrings uci_moves = {};
    for (move m : moves)
    {
        MoveList move_list;
        if (check_move_causes_check(board_state, m, move_list))
        {
            continue;
        }
        uci_moves.push_back(move_to_uci_str(m));
    }
    return uci_moves;
}

inline UCIStrings generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{

    C_BoardState board_state = C_BoardState();
    marshall_board_state(board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    return get_uci_moves(board_state);
}

inline void generate_moves_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    C_BoardState board_state = C_BoardState();

    marshall_board_state(board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    MoveList moves;
    for (int _ = 0; _ < runs; _++)
    {
        collect_legal_moves(board_state, moves);
    }
}

inline float evaluate_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    C_BoardState board_state = C_BoardState();

    marshall_board_state(board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    for (int _ = 0; _ < runs - 1; _++)
    {
        evaluate(board_state);
    }
    return evaluate(board_state);
}

inline float get_best_move(C_Session &session, int max_depth)
{
    printf("Alpha-Beta at depth %i:", max_depth);
    float best_move_score = alphabeta(session, max_depth);

    printf("Reached %d nodes\n", session.alpha_beta_state.nodes_at_depth[max_depth]);
    printf("Bestmove has a score of %f\n", best_move_score);

    printf("%s\n", move_to_uci_str(session.alpha_beta_state.bestmove).c_str());
    print_optimal_line(session, session.alpha_beta_state.current_best_line);

    return best_move_score;
}

inline float get_expected_computation_time(C_Session &session)
{
    int current_depth = session.alpha_beta_state.current_max_depth;
    float branching_factor = session.alpha_beta_state.nodes_at_depth[current_depth - 1] / session.alpha_beta_state.nodes_at_depth[current_depth - 2];
    return session.alpha_beta_state.runtimes_at_depth[current_depth - 1] * branching_factor;
}

RunInfo bestmove(float remaining_time, int max_depth, Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_Session session = construct_session(max_depth);
    marshall_board_state(session.board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    compute_hash(session.hash_state, session.board_state);

    int phase = get_phase(session.board_state);
    float timing_factor = ((initial_phase_weights - phase) * move_length_factor[OPENING] + (phase * move_length_factor[ENDGAME])) / initial_phase_weights;
    float move_time_budget = remaining_time * timing_factor;
    std::string uci_best_move;
    float score;
    do
    {
        auto ts_start = std::chrono::steady_clock::now();
        score = get_best_move(session, session.alpha_beta_state.current_max_depth);
        auto ts_end = std::chrono::steady_clock::now();
        float time_elapsed = (std::chrono::duration_cast<std::chrono::microseconds>(ts_end - ts_start).count()) / 1000000.0;
        session.alpha_beta_state.runtimes_at_depth[session.alpha_beta_state.current_max_depth] = time_elapsed;
        move_time_budget -= time_elapsed;
        ++session.alpha_beta_state.current_max_depth;

        uci_best_move = move_to_uci_str(session.alpha_beta_state.bestmove);

    } while (session.alpha_beta_state.current_max_depth < max_depth && (session.alpha_beta_state.current_max_depth < 5 || get_expected_computation_time(session) < move_time_budget));
    RunInfo results = {uci_best_move.c_str(), session.alpha_beta_state.nodes_at_depth[max_depth]};
    return results;
}

RunInfo bestmove_benchmark(int max_depth, Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_Session session = construct_session(max_depth);
    marshall_board_state(session.board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    compute_hash(session.hash_state, session.board_state);

    std::string uci_best_move;
    float score;

    do
    {
        score = get_best_move(session, session.alpha_beta_state.current_max_depth);
        ++session.alpha_beta_state.current_max_depth;
        uci_best_move = move_to_uci_str(session.alpha_beta_state.bestmove);

    } while (session.alpha_beta_state.current_max_depth <= max_depth);

    RunInfo results = {uci_best_move.c_str(), session.alpha_beta_state.nodes_at_depth[max_depth]};
    return results;
}

#endif