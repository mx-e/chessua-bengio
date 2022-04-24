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
    virtual Directions get_directions(Position position);

    virtual int get_steps();

    explicit Piece(int piece_id);

    int get_id();

protected:
    int piece_id;
};

Piece::Piece(int piece_id)
{
    this->piece_id = piece_id;
}

int Piece::get_id()
{
    return this->piece_id;
}

class Pawn : public Piece
{
    using Piece::Piece;

    Directions get_directions(Position position)
    {
        Directions directions = {{0, 1}};
        if (position.first == 0 && position.second == 1)
        {
            directions.push_back({0, 2});
        }
        return directions;
    }

    int get_steps()
    {
        return 1;
    }
};

class King : public Piece
{
    using Piece::Piece;

    Directions get_directions(Position position)
    {
        return {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    }

    int get_steps()
    {
        return 1;
    }
};

const Piece PIECES[4] = { Pawn(-1), King(-2), Pawn(1), King(2) };

void add_boards_along(Boards boards, Board board, Direction direction, Position position, Piece piece)
{
    position = position + direction;
    int steps = piece.get_steps();

    while (on_board(position) && is_free(position, board) && steps > 0)
    {
        Board new_board = board;
        new_board[position.first].at(position.second) = piece.get_id();
        boards.push_back(new_board);

        position = position + direction;
        steps -= 1;
    }
}

void add_boards_for_piece(Boards boards, Board board, Piece piece, int x, int y)
{
    Position position(x, y);
    Board board_template = board;
    board_template[x].at(y) = 0;

    for (auto direction : piece.get_directions(position))
    {
        add_boards_along(boards, board_template, direction, position, piece);
    }
}