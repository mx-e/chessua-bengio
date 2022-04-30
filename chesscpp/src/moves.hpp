#ifndef MOVES_H
#define MOVES_H
#include "pieces.hpp"

class Piece;

class Move
{
public:
    virtual void step(Position &position) = 0;
    virtual void update(Board &board, BoardState boardState, Piece &piece, Position position) = 0;
};

class DirectionalMove : public Move
{
public:
    DirectionalMove(Direction direction);
    virtual void step(Position &position);
    virtual void update(Board &board, BoardState boardState, Piece &piece, Position position);

private:
    Direction direction;
};

enum CastleSide { KingSide, QueenSide };

class Castle : public Move
{
public:
    Castle(CastleSide side);
    virtual void step(Position &position);
    virtual void update(Board &board, BoardState boardState, Piece &piece, Position position);

private:
    CastleSide side;
};

#endif