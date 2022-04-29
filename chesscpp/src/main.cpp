#include "main.hpp"

Board get_board()
{
    Board m = {};
    return m;
}

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.first + y.first);
}

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

    for (auto direction : piece.get_directions(position))
    {
        add_boards_along(boards, board, direction, position, piece);
    }
}

template <class T>
void add(AvailablePieces &pieces, int color)
{
    std::shared_ptr<Piece> piece = std::make_shared<T>();
    pieces[piece->get_id()] = piece;
}

AvailablePieces get_available_pieces()
{
    AvailablePieces pieces;
    for (auto color : {-1, 1})
    {
        add<King>(pieces, color);
        add<Queen>(pieces, color);
        add<Bishop>(pieces, color);
        add<Knight>(pieces, color);
        add<Rook>(pieces, color);
        add<Pawn>(pieces, color);
    }
    return pieces;
}

Boards get_possible_boards(Board board, int color)
{

    AvailablePieces p = get_available_pieces();

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
