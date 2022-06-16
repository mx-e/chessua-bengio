#include "transforms.hpp"
#include "collections.hpp"
#include "constants.hpp"
#include "evaluations.hpp"
#include <chrono>

typedef std::pair<int, int> Position;
typedef std::vector<Position> EnPassants;

inline void to_bitboard(C_BoardState &board_state, Board mailbox)
{
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
            set_single_piece(board_state, color, type, position_idx);
        }
    }
};

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

inline C_BoardState marshall_board_state(C_BoardState &board_state, Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    to_bitboard(board_state, board);
    board_state.turn = color;
    board_state.half_moves = halfMove;
    board_state.moves = fullMove;

    if (kingSideWhite)
    {
        set_castling_rights(board_state, w_kingside);
    }
    else
    {
        unset_castling_rights(board_state, w_kingside);
    }

    if (kingSideBlack)
    {
        set_castling_rights(board_state, b_kingside);
    }
    else
    {
        unset_castling_rights(board_state, b_kingside);
    }

    if (queenSideWhite)
    {
        set_castling_rights(board_state, w_queenside);
    }
    else
    {
        unset_castling_rights(board_state, w_queenside);
    }

    if (queenSideBlack)
    {
        set_castling_rights(board_state, b_queenside);
    }
    else
    {
        unset_castling_rights(board_state, b_queenside);
    }

    if (enpassant.size() > 0)
    {
        board_state.en_passant = enpassant[0].first * 8 + enpassant[0].second;
    }
    return board_state;
}

inline UCIStrings generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{

    C_BoardState board_state = C_BoardState();
    marshall_board_state(board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    return get_uci_moves(board_state);
}

inline void runtime_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    C_BoardState board_state = C_BoardState();

    marshall_board_state(board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    MoveList moves;
    for (int _ = 0; _ < runs; _++)
    {
        collect_legal_moves(board_state, moves);
    }
}

void print_optimal_move_sequence(MoveList &move_list)
{
    for (move m : move_list)
    {
        printf("%s -> ", move_to_uci_str(m).c_str());
    }

    printf("\n");
}

void print_optimal_line(C_Session &session, Line &line)
{
    for (int i = 0; i < session.alpha_beta_state.current_max_depth; i++)
    {
        printf("%s -> ", move_to_uci_str(line.argmove[i]).c_str());
    }
    std::cout << "\n";
}

float q_search_dummy(C_Session &session, float alpha, float beta, int depth)
{
    if (session.board_state.king_attack || session.board_state.castling_move_illegal)
        return high_value;
    return evaluate(session.board_state);
}

float quiescence_search(C_Session &session, float alpha, float beta, int depth)
{
    if (session.board_state.king_attack || session.board_state.castling_move_illegal)
        return high_value;
    float current_score = evaluate(session.board_state);
    if (depth >= max_quiesence_depth)
        return current_score;
    if (current_score >= beta)
        return beta;
    if (alpha < current_score)
        alpha = current_score;
    MoveList &move_list = session.move_list_stack[depth];
    for (move m : move_list)
    {
        if (m.capture == 0)
            continue;
        ++session.alpha_beta_state.nodes_at_depth[session.alpha_beta_state.current_max_depth];
        push_move(session.board_state, m, session.move_list_stack[depth + 1]);
        float score = -quiescence_search(session, -beta, -alpha, depth + 1);
        pop_move(session.board_state);
        session.move_list_stack[depth + 1].clear();

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

float zw_search(C_Session &session, float beta, int depth_left)
{
    int depth = session.alpha_beta_state.current_max_depth - depth_left;
    if (session.board_state.king_attack || session.board_state.castling_move_illegal)
        return high_value;
    if (depth_left == 0)
        return quiescence_search(session, beta - 1, beta, depth);

    MoveList &move_list = session.move_list_stack[depth];

    for (move m : move_list)
    {
        ++session.alpha_beta_state.nodes_at_depth[session.alpha_beta_state.current_max_depth];
        push_move(session.board_state, m, session.move_list_stack[depth + 1]);

        float score = -zw_search(session, 1 - beta, depth_left - 1);

        pop_move(session.board_state);
        session.move_list_stack[depth + 1].clear();

        if (score >= beta)
            return beta;
    }

    return beta - 1;
}

bool check_check_mate(C_Session &session, int depth)
{
    uint8_t king_field = scan_board(get_king(session.board_state, session.board_state.turn)).front();
    move king_non_move = create_move(king_field, king_field);
    push_move(session.board_state, king_non_move, session.move_list_stack[depth + 1]);
    bool is_check_mate = session.board_state.king_attack;
    pop_move(session.board_state);
    session.move_list_stack[depth + 1].clear();
    return is_check_mate;
}

float pv_search(C_Session &session, float alpha, float beta, int depth_left, Line &pline)
{
    Line line;
    int depth = session.alpha_beta_state.current_max_depth - depth_left;
    if (session.board_state.king_attack || session.board_state.castling_move_illegal)
        return high_value;
    if (depth_left == 0)
    {
        pline.n_moves = 0;
        return quiescence_search(session, alpha, beta, depth);
    }

    MoveList &move_list = session.move_list_stack[depth];

    bool search_pv = true;
    float score;

    for (move m : move_list)
    {
        ++session.alpha_beta_state.nodes_at_depth[session.alpha_beta_state.current_max_depth];
        push_move(session.board_state, m, session.move_list_stack[depth + 1]);

        if (search_pv)
        {
            score = -pv_search(session, -beta, -alpha, depth_left - 1, line);
        }
        else
        {
            score = -zw_search(session, -alpha, depth_left - 1);
            if (score > alpha)
                score = -pv_search(session, -beta, -alpha, depth_left - 1, line);
        }
        pop_move(session.board_state);
        session.move_list_stack[depth + 1].clear();

        if (score >= beta)
            return beta;

        if (score > alpha)
        {
            pline.argmove[0] = m;
            ::memcpy(pline.argmove + 1, line.argmove, line.n_moves * sizeof(move));
            pline.n_moves = line.n_moves + 1;
            alpha = score;
            search_pv = false;
        }
    }
    return alpha;
}

inline int alphabeta(C_Session &session, int max_depth)
{
    collect_legal_moves(session.board_state, session.move_list_stack[0]);
    float best_move_score = pv_search(session, -infty, infty, max_depth, session.alpha_beta_state.current_best_line);
    session.move_list_stack[0].clear();
    session.alpha_beta_state.bestmove = session.alpha_beta_state.current_best_line.argmove[0];
    print_optimal_line(session, session.alpha_beta_state.current_best_line);

    return best_move_score;
}

inline float get_best_move(C_Session &session, int max_depth)
{
    printf("Alpha-Beta at depth %i:", max_depth);
    float best_move_score = alphabeta(session, max_depth);

    printf("Reached %d nodes\n", session.alpha_beta_state.nodes_at_depth[max_depth]);
    printf("Bestmove has a score of %f\n", best_move_score);

    printf("%s\n", move_to_uci_str(session.alpha_beta_state.bestmove).c_str());

    return best_move_score;
}

inline float get_expected_computation_time(C_Session &session)
{
    int current_depth = session.alpha_beta_state.current_max_depth;
    float branching_factor = session.alpha_beta_state.nodes_at_depth[current_depth - 1] / session.alpha_beta_state.nodes_at_depth[current_depth - 2];
    return session.alpha_beta_state.runtimes_at_depth[current_depth - 1] * branching_factor;
}

std::string bestmove(float remaining_time, int max_depth, Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_Session session = construct_session(max_depth);
    marshall_board_state(session.board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    int expected_remaining_moves = std::max(expected_moves_per_game - fullMove, 6);
    float move_time_budget = remaining_time / expected_remaining_moves;
    std::string uci_best_move;

    do
    {
        auto ts_start = std::chrono::steady_clock::now();
        float score = get_best_move(session, session.alpha_beta_state.current_max_depth);
        auto ts_end = std::chrono::steady_clock::now();
        float time_elapsed = (std::chrono::duration_cast<std::chrono::microseconds>(ts_end - ts_start).count()) / 1000000.0;
        session.alpha_beta_state.runtimes_at_depth[session.alpha_beta_state.current_max_depth] = time_elapsed;
        move_time_budget -= time_elapsed;
        ++session.alpha_beta_state.current_max_depth;

        uci_best_move = move_to_uci_str(session.alpha_beta_state.bestmove);

    } while (session.alpha_beta_state.current_max_depth < max_depth && (session.alpha_beta_state.current_max_depth < 5 || get_expected_computation_time(session) < move_time_budget));
    return uci_best_move;
}

std::string bestmove_benchmark(int max_depth, Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_Session session = construct_session(max_depth);
    marshall_board_state(session.board_state, board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    std::string uci_best_move;

    do
    {
        float score = get_best_move(session, session.alpha_beta_state.current_max_depth);
        ++session.alpha_beta_state.current_max_depth;
        uci_best_move = move_to_uci_str(session.alpha_beta_state.bestmove);

    } while (session.alpha_beta_state.current_max_depth <= max_depth);
    return uci_best_move;
}