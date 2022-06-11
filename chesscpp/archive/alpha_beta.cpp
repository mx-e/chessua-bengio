session.alpha_beta_state.reached_nodes++;
float turn = session.board_state.turn;

if (session.board_state.king_attack || session.board_state.castling_move_illegal)
    return std::numeric_limits<int>::min() * turn;

if (session.alpha_beta_state.depth == session.alpha_beta_state.max_depth)
    return evaluate(session.board_state);

MoveList &move_list = session.move_list_stack[session.alpha_beta_state.depth];

int value = std::numeric_limits<int>::min() * turn;
for (move m : move_list)
{
    MoveList next_move_list = session.move_list_stack[session.alpha_beta_state.depth + 1];
    push_move(session.board_state, m, next_move_list);

    ++session.alpha_beta_state.depth;
    int new_value = alphabeta(session);
    --session.alpha_beta_state.depth;

    if (new_value > value * turn)
    {
        value = new_value;
        session.alpha_beta_state.bestmove[session.alpha_beta_state.depth] = m;
    }
    pop_move(session.board_state);
    next_move_list.clear();

    if (turn == White)
    {
        if (value >= session.alpha_beta_state.beta)
            return session.alpha_beta_state.beta;

        session.alpha_beta_state.alpha = max(session.alpha_beta_state.alpha, value);
    }
    else
    {
        if (value <= session.alpha_beta_state.alpha)
            return session.alpha_beta_state.alpha;

        session.alpha_beta_state.beta = min(session.alpha_beta_state.beta, value);
    }
}

return value;