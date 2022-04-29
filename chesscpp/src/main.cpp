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
    this->previous_position = this->position;
    move.step(this->position);
}

bool BoardHead::is_done()
{
    return !(on_board(this->position) && is_free(this->position, this->board) && this->steps > 0);
}

Board BoardHead::do_move()
{
    this->move.update(this->board, this->piece, this->position);
    this->board[this->previous_position.first].at(this->previous_position.second) = 0;

    this->steps -= 1;
    this->update_positions();

    Board board_copy = this->board;
    return board_copy;
}

void add_boards_along(Boards &boards, Board board, Direction direction, Position position, Piece &piece)
{

    DirectionalMove move { direction };
    BoardHead head { board, piece, position, move };

    //Position previous = position;
    //position = position + direction;
    //int steps = piece.get_steps();

    while (!head.is_done())//(on_board(position) && is_free(position, board) && steps > 0)
    {
        //board[previous.first].at(previous.second) = 0;
        //board[position.first].at(position.second) = piece.get_id();

        Board board_copy = head.do_move();

        //Board new_board = board;
        boards.push_back(board_copy);

        //previous = position;
        //position = position + direction;
        //steps -= 1;
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
