#ifndef MOVES_H
#define MOVES_H
#include "pieces.hpp"

class Piece;

class Move
{
public:
    virtual void step() = 0;
    virtual bool is_possible(BoardState boardState);
    virtual void update(Board &board, BoardState boardState, Piece &piece) = 0;

protected:
    Position position;
    bool captured = false;
};

class DirectionalMove : public Move
{
public:
    DirectionalMove(Direction direction, Position position);
    virtual void step();
    virtual void update(Board &board, BoardState boardState, Piece &piece);

private:
    Direction direction;
    Position previous;
};

enum CastleSide { KingSide, QueenSide };

class Castle : public Move
{
public:
    Castle(CastleSide side, Position position);
    virtual void step();
    virtual void update(Board &board, BoardState boardState, Piece &piece);

private:
    CastleSide side;
    Position previous;
};

#endif