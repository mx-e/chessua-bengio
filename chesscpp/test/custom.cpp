#include "custom.hpp"

void fill_column_with_pawns(Board &board, int column, int color)
{
    for(int x = 0; x < 8; x++)
    {
        board[x].at(column) = color * 6;
    }
}

void fill_row_with_pawns(Board &board, int row, int color)
{
    for(int x = 0; x < 8; x++)
    {
        board[row].at(x) = color * 6;
    }
}

bool has_castle(Boards boards, CastleSide side, int color)
{
    bool exists;
    int col = color == COLOR_WHITE ? 0 : 7;
    int kingPosition = side == KingSide ? 6 : 2;
    int rookPosition = side == KingSide ? 5 : 3;

    for (auto board : boards)
    {
        exists = exists || (board[kingPosition][col] == color * 1 && board[rookPosition][col] == color * 5);
    }
    return exists;
}

bool exists(Boards boards, std::function<bool(Board)> boardCondition)
{
    bool found;
    for (auto board : boards)
    {
        found = found || boardCondition(board);
    }
    return found;
}