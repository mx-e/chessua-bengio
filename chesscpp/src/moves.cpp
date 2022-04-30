#include "moves.hpp"

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.second + y.second);
}

DirectionalMove::DirectionalMove(Direction direction)
{
    this->direction = direction;
}

void DirectionalMove::step(Position &position)
{
    position = position + direction;
}

void DirectionalMove::update(Board &board, BoardState boardState, Piece &piece, Position position)
{
    board[position.first].at(position.second) = piece.get_id();
}

Castle::Castle(CastleSide side)
{
    this->side = side;
}

void Castle::step(Position &position)
{
    Direction direction = {side == KingSide ? 2 : -2, 0};
    position = position + direction;
}

void Castle::update(Board &board, BoardState boardState, Piece &piece, Position position)
{
    int color = piece.get_color();
    Position rookPosition = {side == KingSide ? 7 : 0, color == COLOR_WHITE ? 0 : 7};
    Rook utilityRook{color};

    board[position.first].at(position.second) = piece.get_id();

    board[rookPosition.first].at(rookPosition.second) = 0;
    board[position.first + (side == KingSide ? -1 : 1) * 1].at(position.second) = utilityRook.get_id();
}