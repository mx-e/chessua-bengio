#include "pieces.hpp"

class Move
{
public:
    virtual void step(Position &position) = 0;
    virtual void update(Board &board, Piece &piece, Position position) = 0;
};

class DirectionalMove : public Move
{
public:
    DirectionalMove(Direction direction);
    virtual void step(Position &position);
    virtual void update(Board &board, Piece &piece, Position position);

private:
    Direction direction;
};