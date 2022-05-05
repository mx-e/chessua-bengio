#include "moves.hpp"

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

BoardState prepare_board_state(BoardState boardState)
{
    BoardState newState{.board = boardState.board, .color = -boardState.color, .fullMove = boardState.fullMove + 1};
    return newState;
}

bool Move::is_possible(BoardState boardState)
{
    return on_board(position, *boardState.window) && is_free(position, boardState) && !captured;
}

void Move::transfer(BoardStates &boardStates, BoardState newState, BoardState oldState)
{
    newState.halfMove = captured ? 0 : oldState.halfMove + 1;
    boardStates.push_back(newState);
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

void PawnOpeningMove::transfer(BoardStates &boardStates, BoardState newState, BoardState oldState)
{
    newState.enpassant = EnPassants{{previous.first, previous.second - oldState.color * 1}};
    Move::transfer(boardStates, newState, oldState);
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

void Castle::step()
{
    previous = position;

    Direction direction = {side == KingSide ? 2 : -2, 0};
    Move::position = position + direction;
}

void Castle::update(Board &board, BoardState boardState, Piece &piece)
{
    int color = piece.get_color();

    Position rook_position = {position.first + (side == KingSide ? -1 : 1) * 1, position.second};
    Position rook_previous = {side == KingSide ? 7 : 0, color == COLOR_WHITE ? 0 : 7};
    Rook utilityRook{color};

    move(board, piece, previous, position);
    move(board, utilityRook, rook_previous, rook_position);
}

BoardState castle(BoardState boardState, Position rook_position, CastleSide side)
{
    int color = boardState.color;
    Position king_position = rook_position + Direction{side == KingSide ? 1 : -1, 0};
    Position rook_previous = {side == KingSide ? 7 : 0, color == COLOR_WHITE ? 0 : 7};
    Rook rook{color};
    King king{color};

    move(boardState.board, king, rook_position, king_position);
    move(boardState.board, rook, rook_previous, rook_position);

    BoardState newState = prepare_board_state(boardState);
    newState.halfMove++;
    return newState;
}

CastleTransitMove::CastleTransitMove(Direction direction, Position position, CastleSide side = KingSide) : DirectionalMove(direction, position)
{
    this->side = side;
}

void CastleTransitMove::transfer(BoardStates &boardStates, BoardState newState, BoardState oldState)
{
    BoardState castleState = castle(oldState, previous, side);
    newState.dependentState = castleState;
    boardStates.push_back(castleState);
    DirectionalMove::transfer(boardStates, newState, oldState);
}
