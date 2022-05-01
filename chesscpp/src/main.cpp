#include "main.hpp"

Board get_board()
{
    Board m = {};
    return m;
}

class BoardHead
{
public:
    BoardHead(BoardState boardState, Piece &piece, Move &move);
    Board do_move();
    bool is_done();

private:
    void step();
    BoardState boardState;
    Piece &piece;
    Move &move;
    int steps;
};

BoardHead::BoardHead(BoardState boardState, Piece &piece, Move &move) : piece(piece), move(move)
{
    this->boardState = boardState;
    this->piece = piece;
    this->steps = piece.get_steps();

    this->move = move;
    move.step();
}

bool BoardHead::is_done()
{
    return !move.is_possible(boardState) || steps <= 0;
}

Board BoardHead::do_move()
{
    move.update(boardState.board, boardState, piece);
    step();

    Board board_copy = boardState.board;
    return board_copy;
}

void BoardHead::step()
{
    move.step();
    steps--;
}

void add_boards_along(Boards &boards, BoardState boardState, Move &move, Piece &piece)
{
    BoardHead head{boardState, piece, move};

    while (!head.is_done())
    {
        boards.push_back(head.do_move());
    }
}

void add_boards_for_piece(Boards &boards, BoardState boardState, Piece &piece, int x, int y)
{
    Position position(x, y);

    for (auto move : piece.get_moves(boardState, position))
    {
        add_boards_along(boards, boardState, *move, piece);
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
    for (auto color : {COLOR_BLACK, COLOR_WHITE})
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

Boards get_possible_boards(BoardState boardState)
{
    AvailablePieces pieces = get_available_pieces();
    Boards boards;
    Board board = boardState.board;
    int color = boardState.color;

    if(!boardState.window)
    {
        boardState.window = Window{ 0, 0, 8, 8 };
    }

    for (int x = boardState.window->x; x < boardState.window->x_; x++)
    {
        for (int y = boardState.window->y; y < boardState.window->y_; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *pieces[board[x][y]];
                add_boards_for_piece(boards, boardState, piece, x, y);
            }
        }
    }
    return boards;
}
