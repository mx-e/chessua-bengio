
#include "transforms.hpp"
#include "collections.hpp"

typedef std::pair<int, int> Position;
typedef std::vector<Position> EnPassants;
struct move_result
{
    int score;
    std::string move;
};

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
int evaluate(Board board);
int alphabeta(int node, int depth, int alpha, int beta, int color);
std::string bestmove(BoardState boardState, int depth, bool alphaBetaEnabled);

std::string runtime_benchmark_bestmove(bool alphaBetaEnabled, Board board, int color, int depth, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove);
void runtime_benchmark_evaluate(Board board, int runs);

#endif
