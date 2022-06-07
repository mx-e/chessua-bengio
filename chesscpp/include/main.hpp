
#include "transforms.hpp"
#include "collections.hpp"

typedef std::pair<int, int> Position;
typedef std::vector<Position> EnPassants;

inline C_BoardState to_bitboard(Board mailbox)
{
    C_BoardState board_state;

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
    return board_state;
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

inline C_BoardState marshall_board_state(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_BoardState board_state = to_bitboard(board);
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
    C_BoardState board_state = marshall_board_state(board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    return get_uci_moves(board_state);
}

inline void runtime_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    C_BoardState board_state = marshall_board_state(board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    MoveList moves;
    for (int _ = 0; _ < runs; _++)
    {
        collect_legal_moves(board_state, moves);
    }
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a <= b)
        return a;
    return b;
}

// https://en.wikipedia.org/wiki/Hamming_weight
int count_pieces(const C_BoardState &board, const float color, const uint8_t piece_type)
{
    uint64_t x = get_pieces(board, color, piece_type);
    int count = 0;
    for (count = 0; x; count++)
        x &= x - 1;

    return count;
}

void print_move_list(MoveList &move_list, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        printf("%s -> ", move_to_uci_str(move_list[i]).c_str());
    }

    printf("\n");
}

int evaluate(C_BoardState &board_state)
{
    int score = 0;

    score += count_pieces(board_state, 1., b_pawns);
    score += -count_pieces(board_state, -1., b_pawns);

    score += 3 * count_pieces(board_state, 1., b_bishops);
    score += -3 * count_pieces(board_state, -1., b_bishops);

    score += 3 * count_pieces(board_state, 1., b_knights);
    score += -3 * count_pieces(board_state, -1., b_knights);

    score += 5 * count_pieces(board_state, 1., b_rooks);
    score += -5 * count_pieces(board_state, -1., b_rooks);

    score += 9 * count_pieces(board_state, 1., b_queens);
    score += -9 * count_pieces(board_state, -1., b_queens);

    return score;
}

int alphabeta(C_Session &session)
{
    session.alpha_beta_state.reached_nodes++;
    MoveList x = {};
    x.reserve(20);

    if (session.board_state.king_attack || session.board_state.castling_move_illegal)
    {
        if (session.board_state.turn == 1.)
            return std::numeric_limits<int>::max();
        return std::numeric_limits<int>::min();
    }

    if (get_king(session.board_state, session.board_state.turn) == 0)
    {
        printf("No king\n");
        print_move_list(session.board_state.move_stack, session.board_state.move_stack.size());
    }

    if (session.alpha_beta_state.depth == 0)
        return evaluate(session.board_state);

    MoveList &move_list = session.move_list_stack[session.alpha_beta_state.depth];
    collect_legal_moves(session.board_state, move_list);

    int value = 0;
    if (session.board_state.turn == 1.)
    {
        value = std::numeric_limits<int>::min();
        for (move m : move_list)
        {
            push_move(session.board_state, m, x);

            session.alpha_beta_state.depth--;
            int new_value = alphabeta(session);
            session.alpha_beta_state.depth++;

            if (new_value > value)
            {
                value = new_value;
                session.alpha_beta_state.bestmove[session.alpha_beta_state.depth] = m;
            }
            pop_move(session.board_state, x);

            if (value >= session.alpha_beta_state.beta)
                break;

            session.alpha_beta_state.alpha = max(session.alpha_beta_state.alpha, value);
        }
    }
    else
    {
        value = std::numeric_limits<int>::max();
        for (move m : move_list)
        {
            push_move(session.board_state, m, x);

            session.alpha_beta_state.depth--;
            int new_value = alphabeta(session);
            session.alpha_beta_state.depth++;

            if (new_value < value)
            {
                value = new_value;
                session.alpha_beta_state.bestmove[session.alpha_beta_state.depth] = m;
            }
            pop_move(session.board_state, x);

            if (value <= session.alpha_beta_state.alpha)
                break;

            session.alpha_beta_state.alpha = min(session.alpha_beta_state.beta, value);
        }
    }

    return value;
}

std::string bestmove(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    struct C_Session session;
    int depth = 4;

    AlphaBetaState alphabeta_state;
    alphabeta_state.alpha = std::numeric_limits<int>::min();
    alphabeta_state.beta = std::numeric_limits<int>::max();
    alphabeta_state.depth = depth;
    alphabeta_state.reached_nodes = 0;
    alphabeta_state.bestmove.resize(depth);
    session.alpha_beta_state = alphabeta_state;

    C_BoardState board_state = marshall_board_state(board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    session.board_state = board_state;

    UCIStrings x = get_uci_moves(board_state);

    MoveListStack move_list_stack;
    reserve_move_list_stack(move_list_stack);
    session.move_list_stack = move_list_stack;

    int bestmove = alphabeta(session);

    printf("Reached %d nodes\n", session.alpha_beta_state.reached_nodes);
    printf("Bestmove has a score of %d\n", bestmove);

    print_move_list(session.alpha_beta_state.bestmove, depth);
    print_move_list(session.board_state.move_stack, depth);
    printf("%s\n", move_to_uci_str(session.alpha_beta_state.bestmove[depth]).c_str());

    return move_to_uci_str(session.alpha_beta_state.bestmove[depth]);
}
