#include "transforms.hpp"

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