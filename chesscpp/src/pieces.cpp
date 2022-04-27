#include "pieces.hpp"

Piece::Piece(int id)
{
    this->id = id;
}

int Piece::get_id()
{
    return this->id;
}