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
    BoardState do_move();
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

BoardState BoardHead::do_move()
{
    move.update(boardState.board, boardState, piece);
    step();

    BoardState newState{.board = boardState.board, .color = -boardState.color, .fullMove = boardState.fullMove + 1};

    move.transfer(newState, boardState);
    return newState;
}

void BoardHead::step()
{
    move.step();
    steps--;
}

void add_boards_along(BoardStates &boardStates, BoardState boardState, Move &move, Piece &piece)
{
    BoardHead head{boardState, piece, move};

    while (!head.is_done())
    {
        boardStates.push_back(head.do_move());
    }
}

void add_boards_for_piece(BoardStates &boardStates, BoardState boardState, Piece &piece, int x, int y)
{
    Position position(x, y);

    for (auto move : piece.get_moves(boardState, position))
    {
        add_boards_along(boardStates, boardState, *move, piece);
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

BoardStates get_possible_boards(BoardState boardState)
{
    AvailablePieces pieces = get_available_pieces();
    BoardStates boardStates;
    Board board = boardState.board;
    int color = boardState.color;

    if (!boardState.window)
    {
        boardState.window = Window{0, 0, 8, 8};
    }

    for (int x = boardState.window->x; x < boardState.window->x_; x++)
    {
        for (int y = boardState.window->y; y < boardState.window->y_; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *pieces[board[x][y]];
                add_boards_for_piece(boardStates, boardState, piece, x, y);
            }
        }
    }
    return boardStates;
}
