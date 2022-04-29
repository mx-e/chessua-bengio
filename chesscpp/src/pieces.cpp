#include "pieces.hpp"

Piece::Piece(int color)
{
    this->color = color;
}

int Piece::get_color()
{
    return color;
}

Directions King::get_directions(Position position)
{
    return {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}};
}

int King::get_steps() { return 1; }
int King::get_id() { return color * 1; }

Directions Queen::get_directions(Position position)
{
    return {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}};
}

int Queen::get_steps() { return 8; }
int Queen::get_id() { return color * 2; }

Directions Bishop::get_directions(Position position)
{
    return {{-1, 0}, {1, -1}, {-1, 1}, {1, 1}};
}

int Bishop::get_steps() { return 8; }
int Bishop::get_id() { return color * 3; }

Directions Knight::get_directions(Position position)
{
    return {{2, 1}, {1, 2}, {-2, 1}, {-1, 2}};
}

int Knight::get_steps() { return 1; }
int Knight::get_id() { return color * 4; }

Directions Rook::get_directions(Position position)
{
    return {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
}

int Rook::get_steps() { return 8; }
int Rook::get_id() { return color * 5; }

Directions Pawn::get_directions(Position position)
{
    Directions directions = {{0, 1}};
    if (position.first == 0 && position.second == 1)
    {
        directions.push_back({0, 2});
    }
    return directions;
}

int Pawn::get_steps() { return 1; }
int Pawn::get_id() { return color * 6; }