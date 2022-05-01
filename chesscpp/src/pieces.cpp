#include "pieces.hpp"

Piece::Piece(int color)
{
    this->color = color;
}

int Piece::get_color()
{
    return color;
}

Moves from_directions(Directions directions, Position position)
{
    Moves moves;
    for (auto direction : directions)
    {
        std::shared_ptr<Move> move = std::make_shared<DirectionalMove>(direction, position);
        moves.push_back(move);
    }
    return moves;
}

Moves King::get_moves(BoardState boardState, Position position)
{
    Moves moves = from_directions({{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}}, position);

    ColorCastlingRights castlingRights = boardState.color == COLOR_BLACK ? boardState.castlingRights.black : boardState.castlingRights.white;
    if (castlingRights.kingSide)
    {
        moves.push_back(std::make_shared<Castle>(KingSide, position));
    }

    if (castlingRights.queenSide)
    {
        moves.push_back(std::make_shared<Castle>(QueenSide, position));
    }

    return moves;
}

int King::get_steps() { return 1; }
int King::get_id() { return color * 1; }

Moves Queen::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}}, position);
}

int Queen::get_steps() { return 8; }
int Queen::get_id() { return color * 2; }

Moves Bishop::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {1, -1}, {-1, 1}, {1, 1}}, position);
}

int Bishop::get_steps() { return 8; }
int Bishop::get_id() { return color * 3; }

Moves Knight::get_moves(BoardState boardState, Position position)
{
    return from_directions({{2, 1}, {1, 2}, {-2, 1}, {-1, 2}}, position);
}

int Knight::get_steps() { return 1; }
int Knight::get_id() { return color * 4; }

Moves Rook::get_moves(BoardState boardState, Position position)
{
    return from_directions({{0, 1}, {1, 0}, {0, -1}, {-1, 0}}, position);
}

int Rook::get_steps() { return 8; }
int Rook::get_id() { return color * 5; }

void add_diagonal_captures(Directions &directions, BoardState boardState, Position position)
{
    for (Direction direction : Directions{{1, boardState.color * 1}, {-1, boardState.color * 1}})
    {
        Position diagonal = position + direction;
        if (boardState.color * boardState.board[diagonal.first][diagonal.second] < 0)
        {
            directions.push_back(direction);
        }
    }
}

Moves Pawn::get_moves(BoardState boardState, Position position)
{
    Directions directions = {{0, boardState.color * 1}};
    if (position.second == (boardState.color == COLOR_WHITE ? 1 : 6))
    {
        directions.push_back({0, boardState.color * 2});
    }

    add_diagonal_captures(directions, boardState, position);

    return from_directions(directions, position);
}

int Pawn::get_steps() { return 1; }
int Pawn::get_id() { return color * 6; }