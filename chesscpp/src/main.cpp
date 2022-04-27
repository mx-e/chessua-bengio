#include <array>
#include <vector>
#include <map>
#include <memory>
#include "main.hpp"
#include "pieces.hpp"
#include <iostream>

Board get_board()
{
    Board m = {};
    return m;
}

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.first + y.first);
}

class Pawn : public Piece
{
    using Piece::Piece;

public:
    virtual Directions get_directions(Position position)
    {
        Directions directions = {{0, 1}};
        return directions;

        if (position.first == 0 && position.second == 1)
        {
            directions.push_back({0, 2});
        }
        return directions;
    }

    virtual int get_steps()
    {
        return 1;
    }
};

class King : public Piece
{
    using Piece::Piece;

public:
    virtual Directions get_directions(Position position)
    {
        Directions directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, 1}, {-1, -1}};
        return directions;
    }

    virtual int get_steps()
    {
        return 1;
    }
};

bool on_board(Position position)
{
    return position.first >= 0 && position.first < 8 && position.second >= 0 && position.second < 8;
}

bool is_free(Position position, Board board)
{
    return board[position.first][position.second] == 0;
}

void add_boards_along(Boards &boards, Board board, Direction direction, Position position, Piece &piece)
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

void add_boards_for_piece(Boards &boards, Board board, Piece &piece, int x, int y)
{
    Position position(x, y);
    board[x].at(y) = 0;

    Directions dirs2 = piece.get_directions(position);

    Directions dirs = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (auto direction : piece.get_directions(position))
    {
        add_boards_along(boards, board, direction, position, piece);
    }
}

Boards get_possible_boards(Board board, int color)
{
    std::map<int, std::shared_ptr<Piece> > p = {
        {-2, std::make_shared<King>(-2)},
        {-1, std::make_shared<Pawn>(-1)},
        {1, std::make_shared<Pawn>(1)},
        {2, std::make_shared<King>(2)},
    };

    Boards boards;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *p[board[x][y]];
                add_boards_for_piece(boards, board, piece, x, y);
            }
        }
    }
    return boards;
}
