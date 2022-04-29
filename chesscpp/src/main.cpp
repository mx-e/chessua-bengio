#include "main.hpp"

Board get_board()
{
    Board m = {};
    return m;
}

bool on_board(Position position)
{
    return position.first >= 0 && position.first < 8 && position.second >= 0 && position.second < 8;
}

bool is_free(Position position, Board board)
{
    return board[position.first][position.second] == 0;
}

class BoardHead
{
public:
    BoardHead(Board board, Piece &piece, Position position, Move &move);
    Board do_move();
    bool is_done();

private:
    Board board;
    Piece &piece;
    Position position;
    Position previous_position;
    Move &move;
    int steps;

    void update_positions();
};

BoardHead::BoardHead(Board board, Piece &piece, Position position, Move &move) : piece(piece), move(move)
{
    this->board = board;
    this->piece = piece;
    this->move = move;
    this->position = position;
    this->steps = piece.get_steps();
    this->update_positions();
}

void BoardHead::update_positions()
{
    previous_position = position;
    move.step(position);
}

bool BoardHead::is_done()
{
    return !(on_board(position) && is_free(position, board) && steps > 0);
}

Board BoardHead::do_move()
{
    move.update(board, piece, position);
    board[previous_position.first].at(previous_position.second) = 0;

    steps -= 1;
    update_positions();

    Board board_copy = board;
    return board_copy;
}

void add_boards_along(Boards &boards, Board board, Direction direction, Position position, Piece &piece)
{
    DirectionalMove move{direction};
    BoardHead head{board, piece, position, move};

    while (!head.is_done())
    {
        boards.push_back(head.do_move());
    }
}

void add_boards_for_piece(Boards &boards, Board board, Piece &piece, int x, int y)
{
    Position position(x, y);

    for (auto direction : piece.get_directions(position))
    {
        add_boards_along(boards, board, direction, position, piece);
    }
}

template <class T>
void add(AvailablePieces &pieces, int color)
{
    std::shared_ptr<Piece> piece = std::make_shared<T>(color);
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
    AvailablePieces pieces = get_available_pieces();
    Boards boards;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *pieces[board[x][y]];
                add_boards_for_piece(boards, board, piece, x, y);
            }
        }
    }
    return boards;
}
