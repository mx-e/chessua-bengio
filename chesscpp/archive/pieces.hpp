#ifndef PIECES_H
#define PIECES_H
#include "types.hpp"
#include "session.hpp"
#include "moves.hpp"
#include <memory>
#include <map>
#include <algorithm>

class Move;

typedef std::vector<std::shared_ptr<Move> > Moves;

/**
 * @brief Base class of Pieces of all kinds. A Piece has an id, a color, a UCI character and
 * a number of steps that it can perform (e.g., 1 for the King and up to 8 for the Queen).
 * A Piece produces a list of Moves given the current BoardState and its Position.
 * 
 */
class Piece
{
public:

    /**
     * @brief Construct a new Piece object and assign it its color.
     * 
     * @param color Color of the Piece (-1 for black and 1 for white).
     */
    Piece(int color);

    /**
     * @brief Get the list of Moves given the current BoardState and the Position of Piece.
     * Validity checks fit either into this method or in Move#is_possible depending on the need.
     * 
     * @param boardState Current BoardState
     * @param position Current Position of the Piece
     * @return Moves that the Piece emits according to the current state.
     */
    virtual Moves get_moves(BoardState boardState, Position position) = 0;

    /**
     * @brief Returns the number of steps that the Piece can perform at most (this does not take into accout
     * the current BoardState).
     * 
     * @return int The number of steps that the Piece can perform.
     */
    virtual int get_steps() = 0;

    /**
     * @brief Global id of the Piece that is used to represent the current Board in the BoardState
     * Use this method when needing the id of an arbitrary Piece. Some Pieces expose their ID as 
     * a static value to serve as global constants.
     * @return int The id of the Piece.
     */
    virtual int get_id() = 0;

    /**
     * @brief Get the color of the Piece.
     * 
     * @return int The color of the Piece.
     */
    virtual int get_color();

    /**
     * @brief Get the UCI character associated with this Piece.
     * 
     * @return int The UCI character associated with this Piece.
     */
    virtual int get_character();

protected:
    int color;
};

typedef std::map<int, std::shared_ptr<Piece> > AvailablePieces;

/**
 * @brief Implements the King Piece.
 * 
 */
class King : public Piece
{
public:
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();

 public:
   static const int ID = 1;
};

/**
 * @brief Implements the Queen Piece.
 * 
 */
class Queen : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();

public:
   static const int ID = 2;
};

/**
 * @brief Implements the Bishop Piece.
 * 
 */
class Bishop : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();

public:
    static const int ID = 3;
};

/**
 * @brief Implements the Knight Piece.
 * 
 */
class Knight : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();
    virtual int get_character();

public:
    static const int ID = 4;
};

/**
 * @brief Implements the Rook Piece.
 * 
 */
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

/**
 * @brief Implements the Pawn Piece.
 * 
 */
class Pawn : public Piece
{
    using Piece::Piece;
    virtual Moves get_moves(BoardState boardState, Position position);
    virtual int get_steps();
    virtual int get_id();

public:
    static const int ID = 6;
};

BoardState prepare_board_state(BoardState boardState);

#endif