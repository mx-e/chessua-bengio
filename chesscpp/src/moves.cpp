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

void DirectionalMove::update(Board &board, Piece &piece, Position position)
{
    board[position.first].at(position.second) = piece.get_id();
}