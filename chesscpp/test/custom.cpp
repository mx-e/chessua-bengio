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

bool has_castle(BoardStates boardStates, CastleSide side, int color)
{
    bool exists;
    int col = color == COLOR_WHITE ? 0 : 7;
    int kingPosition = side == KingSide ? 6 : 2;
    int rookPosition = side == KingSide ? 5 : 3;

    for (auto boardState : boardStates)
    {
        exists = exists || (boardState.board[kingPosition][col] == color * 1 && boardState.board[rookPosition][col] == color * 5);
    }
    return exists;
}

bool exists(BoardStates boardStates, std::function<bool(BoardState)> boardStateCondition)
{
    bool found;
    for (auto boardState : boardStates)
    {
        found = found || boardStateCondition(boardState);
    }
    return found;
}