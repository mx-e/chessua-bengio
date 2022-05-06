#ifndef PIECES_H
#define PIECES_H
#include "types.hpp"
#include "moves.hpp"
#include <memory>
#include <map>
#include <algorithm>

class Move;

typedef std::vector<std::shared_ptr<Move> > Moves;

class Piece
{
public:
    Piece(int color);
    virtual Moves get_moves(BoardState boardState, Position position) = 0;
    virtual int get_steps() = 0;
    virtual int get_id() = 0;
    virtual int get_color();
    virtual int get_character();

protected:
    int color;
};

typedef std::map<int, std::shared_ptr<Piece> > AvailablePieces;

class Pawn : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
};

class King : public Piece
{
public:
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    static const int ID = 1;
};

class Queen : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();
};

class Bishop : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();
};

class Knight : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();
};

class Rook : public Piece
{
public:
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();

    static const int ID = 5;
};

BoardState prepare_board_state(BoardState boardState);

#endif