#ifndef MOVES_H
#define MOVES_H
#include "pieces.hpp"

class BoardInCheckException : public std::exception {};

class Piece;

/**
 * @brief Base class abstracting moves that can be performed in chess.
 * A Move interacts with the current BoardState and can move at least the current Piece.
 * The lifetime of a Move depends on the number of Board instances a Piece can produce under the 
 * directive of the Move.
 * @see BoardState
 * @see Piece
 */
class Move
{
public:

    /**
     * @brief Validates the current BoardState and determines whether the given Move is legal 
     * according to chess rules. This can be as basic as that a Piece cannot leave the board.
     * Note that this validation is only meaningful after the step method was called, as state validation
     * is performed look-ahead (first perform a Move#step and then validate the move).
     * @param boardState Current BoardState to validate the move against.
     * @return whether the current BoardState is legal.
     */
    virtual bool is_possible(BoardState boardState);

    /**
     * @brief Updates internal parameters to perform the move. This does not interact with 
     * the BoardState and simply seperates moving, validating and actually mutating the BoardState.
     */
    virtual void step() = 0;

    /**
     * @brief Updates a Board reference given the current BoardState. At this point the Piece that is to be moved
     * is finally exposed to the Move instance. This step ensures that a new Board can be computed
     * given the current Move.
     * 
     * @param board Board that is subject to change under the current Move.
     * @param boardState Current BoardState.
     * @param piece Current Piece.
     */
    virtual void update(Board &board, BoardState boardState, Piece &piece) = 0;

    /**
     * @brief Models the implications that a Move can have on the follow-up BoardState.
     * 
     * @param newState Reference to the new BoardState that resulted from the current Move.
     * @param oldState Old BoardState before the update method was called.
     */
    virtual void transfer(BoardState &newState, BoardState oldState);

    /**
     * @brief Get the position object
     * 
     * @return Position (Potentially updated) position of the Move.
     */
    virtual Position get_position();
protected:

    /**
     * @brief Current Position that the Move moves a Piece through.
     * 
     */
    Position position;

    /**
     * @brief Previous Position before the Move was performed.
     * 
     */
    Position previous;

    /**
     * @brief Internal flag whether the move has lead to a capture.
     * 
     */
    bool captured = false;
};

/**
 * @brief A DirectionaMove is performed along a given direction for up to any number of steps.
 * The idea is move the Piece along the Position until it either captures an opponent or is blocked
 * by the Board boudaries or a friendly Piece.
 */
class DirectionalMove : public Move
{
public:

    /**
     * @brief Construct a new DirectionalMove instance given a Direction to follow and 
     * the current Position of a Piece.
     * 
     * @param direction Direction of the DirectionalMove
     * @param position Position of the current Piece.
     */
    DirectionalMove(Direction direction, Position position);
    virtual void step();
    virtual void update(Board &board, BoardState boardState, Piece &piece);

protected:
    /**
     * @brief Direction along which the DirectionalMove is performed.
     * 
     */
    Direction direction;
};

/**
 * @brief A PawnMove is a DirectionalMove that cannot capture in a straight line and is
 * thus blocked by an opponent and a friendly Piece.
 * 
 */
class PawnMove : public DirectionalMove
{
public:
    using DirectionalMove::DirectionalMove;

    /**
     * @brief Validates the current BoardState and determines whether the given Move is legal 
     * according to chess rules.
     * 
     * @param boardState 
     * @return true if the DirectionalMove#is_possible and the Pawn is not blocked by an opponent
     * straight ahead.
     */
    virtual bool is_possible(BoardState boardState);
};

/**
 * @brief A PawnOpeningMove is a PawnMove that can be performed only once before the Pawn has moved initially.
 * In this case the Pawn can be moved two fields.
 */
class PawnOpeningMove : public PawnMove
{
public:
    using PawnMove::PawnMove;

    /**
     * @brief Adds the en-passant position behind the opening Pawn to the follow-up state.
     * 
     * @param newState Reference to the new BoardState that resulted from the current Move.
     * @param oldState Old BoardState before the update method was called.
     */
    virtual void transfer(BoardState &newState, BoardState oldState);
    virtual bool is_possible(BoardState boardState);
};

/**
 * @brief A PawnSwapMove is a PawnMove that moves to the opposite end of the Board and is thus
 * subject to a promotion
 * 
 */
class PawnSwapMove : public PawnMove
{
public:
    PawnSwapMove(Direction direction, Position position, std::shared_ptr<Piece> swapPiece);
    virtual void update(Board &board, BoardState boardState, Piece &piece);
    virtual void transfer(BoardState &newState, BoardState oldState);

private:
    std::shared_ptr<Piece> swapPiece;
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
    virtual bool is_possible(BoardState boardState);
    virtual void update(Board &board, BoardState boardState, Piece &piece);

private:
    CastleSide side;
};

#endif