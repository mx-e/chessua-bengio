#include "../include/types.hpp"

BoardState::BoardState(Board board, int color, int halfMove, int fullMove)
{
    this->board = board;
    this->color = color;
    this->halfMove = halfMove;
    this->fullMove = fullMove;
    this->castlingRights = CastlingRights{ .white = ColorCastlingRights{false, false}, .black = ColorCastlingRights{false, false}};
    this->window = Window{0, 0, 8, 8};
}

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.second + y.second);
}

