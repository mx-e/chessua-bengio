#ifndef MOVES_H
#define MOVES_H
#include "pieces.hpp"

/**
 * @brief Special exception thrown if a Move would capture the opponent's King.
 * 
 */
class BoardInCheckException : public std::exception {};

class Piece;

enum CastleSide { KingSide, QueenSide };

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
 * @brief A PawnPromotion is a PawnMove that moves to the opposite end of the Board and is thus
 * subject to a promotion (swapped out for a captured Queen, Bishop, Knight or Rook).
 * 
 */
class PawnPromotion : public PawnMove
{
public:

    /**
     * @brief Construct a new PawnPromotion instance as a PawnMove with the additional swapPiece,
     * a reference to a Piece of Queen, Bishop, Knight or Rook, which the Pawn gets promoted to when
     * performing this Move.
     * 
     * @param direction Direction of the DirectionalMove
     * @param position Position of the current Piece.
     * @param swapPiece Reference to either a Queen, Bishop, Knight or Rook which the current Pawn gets promoted to.
     */
    PawnPromotion(Direction direction, Position position, std::shared_ptr<Piece> swapPiece);
    virtual void update(Board &board, BoardState boardState, Piece &piece);
    virtual void transfer(BoardState &newState, BoardState oldState);

private:
    std::shared_ptr<Piece> swapPiece;
};

/**
 * @brief EnPassantCapture is a DirectionalMove that is performed diagonally by a Pawn and the 
 * only chess move that captures a Piece without moving onto its current field. Whether a field is
 * target to an EnPassantCapture is contained in the BoardState.
 * 
 */
class EnPassantCapture : public DirectionalMove
{
public:
    using DirectionalMove::DirectionalMove;
    virtual void update(Board &board, BoardState boardState, Piece &piece);
};

/**
 * @brief A Castle is a special Move that moves the King by two fields and moves the Rook to
 * the position the King has transitioned through. This special Move requires additional constraints, among
 * such that the King does not transit through a check and that the path between King and Rook is 
 * free. Additionally, this Move is only possible if the King and Rook have not yet moved. The latter
 * is configured in the BoardState.
 * 
 */
class Castle : public Move
{
public:

    /**
     * @brief Construct a new Castle instance from a side (QueenSide or KingSide) and a the initial 
     * Position of the King. This Position mostly depends on the current player.
     * 
     * @param side CastleSide (QueenSide or KingSide) indicating whether the Castle happens on the side 
     * of the Queen or the side of the King.
     * @param position Position of the King.
     */
    Castle(CastleSide side, Position position);
    virtual void step();

    /**
     * @brief Validates the current BoardState and determines whether the given Move is legal 
     * according to chess rules.
     * 
     * @param boardState 
     * @return true if the Move#is_possible, the King does not transit through check and the path between
     * the King and the respective Rook is free.
     */
    virtual bool is_possible(BoardState boardState);
    virtual void update(Board &board, BoardState boardState, Piece &piece);

private:
    CastleSide side;
};

#endif