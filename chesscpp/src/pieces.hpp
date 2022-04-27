#include "main.hpp"

class Piece
{
public:
    /**
     * @brief Provides the immediate directions a piece can move, given the current position.
     * Note that the method should return also directions that are invalid given the
     * game state or board limits. In a sense, this method returns all conceivable moves.
     * @param position
     * @return Directions
     */
    virtual Directions get_directions(Position position) = 0;

    virtual int get_steps() = 0;

    Piece(int id);

    virtual ~Piece() {}

    int get_id();

private:
    int id;
};