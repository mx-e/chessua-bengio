#include "../include/moves.hpp"

bool on_board(Position position, Window window)
{
    return position.first >= window.x && position.first < window.x_ && position.second >= window.y && position.second < window.y_;
}

bool is_free(Position position, BoardState boardState)
{
    return boardState.color * boardState.board[position.first][position.second] <= 0;
}

void move(Board &board, Piece &piece, Position previous, Position position)
{
    board[previous.first].at(previous.second) = 0;
    board[position.first].at(position.second) = piece.get_id();
}

void Move::transfer(BoardState &newState, BoardState oldState)
{
    newState.halfMove = captured ? 0 : oldState.halfMove + 1;
    
}

bool Move::is_possible(BoardState boardState)
{
    return on_board(position, boardState.window) && is_free(position, boardState) && !captured;
}

Position Move::get_position()
{
    return position;
}

DirectionalMove::DirectionalMove(Direction direction, Position position)
{
    this->direction = direction;
    this->position = position;
}

void DirectionalMove::step()
{
    previous = position;
    position = position + direction;
}

void DirectionalMove::update(Board &board, BoardState boardState, Piece &piece)
{
    captured = board[position.first].at(position.second) != 0;

    if (board[position.first].at(position.second) == boardState.color * -1)
    {
        throw BoardInCheckException();
    }

    move(board, piece, previous, position);
}

bool PawnMove::is_possible(BoardState boardState)
{
    return Move::is_possible(boardState) && boardState.board[position.first][position.second] == 0;
}

bool PawnOpeningMove::is_possible(BoardState boardState)
{
    int one_back = boardState.color == COLOR_WHITE ? -1 : 1;
    return PawnMove::is_possible(boardState) && boardState.board[position.first][position.second + one_back] == 0;
}

void PawnOpeningMove::transfer(BoardState &newState, BoardState oldState)
{
    newState.enpassant = EnPassants{{position.first, position.second - oldState.color}};
    Move::transfer(newState, oldState);
}

PawnSwapMove::PawnSwapMove(Direction direction, Position position, std::shared_ptr<Piece> swapPiece) : PawnMove(direction, position)
{
    this->swapPiece = swapPiece;
}

void PawnSwapMove::update(Board &board, BoardState boardState, Piece &piece)
{
    PawnMove::update(board, boardState, piece);
    board[position.first].at(position.second) = swapPiece->get_id();
}

void PawnSwapMove::transfer(BoardState &newState, BoardState oldState)
{
    PawnMove::transfer(newState, oldState);
    newState.uci += swapPiece->get_character();
}

void EnPassantCapture::update(Board &board, BoardState boardState, Piece &piece)
{
    captured = true;
    move(board, piece, previous, position);
    board[position.first].at(position.second - boardState.color) = 0;
}

Castle::Castle(CastleSide side, Position position)
{
    this->side = side;
    this->position = position;
}

bool castle_path_free(BoardState boardState, int side)
{
    int start = side == KingSide ? 5 : 1;
    int end = side == KingSide ? 7 : 4;
    int col = boardState.color == COLOR_WHITE ? 0 : 7;
    for (int i = start; i < end; i++)
    {
        if (boardState.board[i][col] != 0)
            return false;
    }
    return true;
}

bool Castle::is_possible(BoardState boardState)
{
    //  TODO: Buffer transit state before and possible boards for transit after
    DirectionalMove transitMove = DirectionalMove(Direction{(side == KingSide ? 1 : -1), 0}, previous);
    King king{boardState.color};
    transitMove.step();

    if (!castle_path_free(boardState, side))
        return false;

    transitMove.update(boardState.board, boardState, king);

    BoardState transitState = prepare_board_state(boardState);
    transitMove.transfer(transitState, boardState);

    // to avoid infinite recursion
    transitState.castlingRights = CastlingRights{};
    try
    {
        get_possible_boards(transitState);
        return true;
    }
    catch (const BoardInCheckException &e)
    {
        return false;
    }
}

void Castle::step()
{
    previous = position;

    Direction direction = {side == KingSide ? 2 : -2, 0};
    Move::position = position + direction;
}

void Castle::update(Board &board, BoardState boardState, Piece &piece)
{
    int color = piece.get_color();

    Position rook_position = {position.first + (side == KingSide ? -1 : 1), position.second};
    Position rook_previous = {side == KingSide ? 7 : 0, color == COLOR_WHITE ? 0 : 7};
    Rook utilityRook{color};

    move(board, piece, previous, position);
    move(board, utilityRook, rook_previous, rook_position);
}
