#include <array>
#include <vector>
#include "main.hpp"
#include "utils.hpp"

float big_loop(int size)
{
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result += 1;
        }
    }
    return (float)result;
}

Board get_board()
{
    Board m = {};
    return m;
}

Board add_1_to_board(Board board)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i].at(j) += 1;
        }
    }
    return board;
}

typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;

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

    explicit Piece(int piece_id)
    {
        this->piece_id = piece_id;
    }

    int get_id()
    {
        return this->piece_id;
    }

protected:
    int piece_id;
};

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

Piece PIECES[4] = { Pawn(-1), King(-2), Pawn(1), King(2) };

bool on_board(Position position)
{
    return position.first >= 0 && position.first < 8 && position.second >= 0 && position.second;
}

bool is_free(Position position, Board board)
{
    return board[position.first][position.second] == 0;
}

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.first + y.first);
}

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

Boards get_possible_boards(Board board, int color)
{
    Boards boards;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece piece = PIECES[board[x][y] + 16];
                add_boards_for_piece(boards, board, piece, x, y);
            }
        }
    }
    return boards;
}
