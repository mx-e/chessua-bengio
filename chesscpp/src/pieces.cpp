#include "pieces.hpp"

Piece::Piece(int color)
{
    this->color = color;
}

int Piece::get_color()
{
    return color;
}

Moves from_directions(Directions directions)
{
    Moves moves;
    for (auto direction : directions)
    {
        std::shared_ptr<Move> move = std::make_shared<DirectionalMove>(direction);
        moves.push_back(move);
    }
    return moves;
}

Moves King::get_moves(BoardState boardState, Position position)
{
    Moves moves = from_directions({{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}});

    ColorCastlingRights castlingRights = boardState.color == COLOR_BLACK ? boardState.castlingRights.black : boardState.castlingRights.white;
    if (castlingRights.kingSide)
    {
        moves.push_back(std::make_shared<Castle>(KingSide));
    }

    if (castlingRights.queenSide) 
    {
        moves.push_back(std::make_shared<Castle>(QueenSide));
    }
        
    return moves;
}

int King::get_steps() { return 1; }
int King::get_id() { return color * 1; }

Moves Queen::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}});
}

int Queen::get_steps() { return 8; }
int Queen::get_id() { return color * 2; }

Moves Bishop::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {1, -1}, {-1, 1}, {1, 1}});
}

int Bishop::get_steps() { return 8; }
int Bishop::get_id() { return color * 3; }

Moves Knight::get_moves(BoardState boardState, Position position)
{
    return from_directions({{2, 1}, {1, 2}, {-2, 1}, {-1, 2}});
}

int Knight::get_steps() { return 1; }
int Knight::get_id() { return color * 4; }

Moves Rook::get_moves(BoardState boardState, Position position)
{
    return from_directions({{0, 1}, {1, 0}, {0, -1}, {-1, 0}});
}

int Rook::get_steps() { return 8; }
int Rook::get_id() { return color * 5; }

Moves Pawn::get_moves(BoardState boardState, Position position)
{
    Directions directions = {{0, 1}};
    if (position.first == 0 && position.second == 1)
    {
        directions.push_back({0, 2});
    }
    return from_directions(directions);
}

int Pawn::get_steps() { return 1; }
int Pawn::get_id() { return color * 6; }