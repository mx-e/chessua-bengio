#ifndef MOVES_H
#define MOVES_H
#include "pieces.hpp"

class BoardInCheckException : public std::exception {};

class Piece;

class Move
{
public:
    virtual bool is_possible(BoardState boardState);
    virtual void step() = 0;
    virtual void update(Board &board, BoardState boardState, Piece &piece) = 0;
    virtual void transfer(BoardStates &boardStates, BoardState newState, BoardState oldState);

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

protected:
    Direction direction;
    Position previous;
};

class PawnMove : public DirectionalMove
{
public:
    using DirectionalMove::DirectionalMove;
    virtual bool is_possible(BoardState boardState);
};

class PawnOpeningMove : public PawnMove
{
public:
    using PawnMove::PawnMove;
    virtual void transfer(BoardStates &boardStates, BoardState newState, BoardState oldState);
};

class EnPassantCapture : public DirectionalMove
{
public:
    using DirectionalMove::DirectionalMove;
    virtual void update(Board &board, BoardState boardState, Piece &piece);
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

class CastleTransitMove : public DirectionalMove
{
public:
    CastleTransitMove(Direction direction, Position position, CastleSide side);
    virtual void transfer(BoardStates &boardStates, BoardState newState, BoardState oldState);
private:
    CastleSide side;
};

#endif