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
    Moves moves;
    Directions base_directions = {{0, -1}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}};

    ColorCastlingRights castlingRights = boardState.color == COLOR_BLACK ? boardState.castlingRights.black : boardState.castlingRights.white;
    if (castlingRights.kingSide)
    {
        std::shared_ptr<Move> kingSideTransit = std::make_shared<CastleTransitMove>(Direction{1, 0}, position, KingSide);
        moves.push_back(kingSideTransit);
    }
    else
    {
        base_directions.push_back(Direction{1, 0});
    }

    if (castlingRights.queenSide)
    {
        std::shared_ptr<Move> queenSideTransit = std::make_shared<CastleTransitMove>(Direction{-1, 0}, position, QueenSide);
        moves.push_back(queenSideTransit);
    }
    else
    {
        base_directions.push_back(Direction{-1, 0});
    }

    for (auto move : from_directions(base_directions, position))
    {
        moves.push_back(move);
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

bool captures_enpassant(BoardState boardState, Position diagonal)
{
    if (boardState.enpassant)
    {
        auto position = std::find(boardState.enpassant->begin(), boardState.enpassant->end(), diagonal);
        return position != boardState.enpassant->end();
    }
    return false;
}

void add_diagonal_captures(Moves &moves, BoardState boardState, Position position)
{
    for (Direction direction : Directions{{1, boardState.color * 1}, {-1, boardState.color * 1}})
    {
        Position diagonal = position + direction;

        if (boardState.color * boardState.board[diagonal.first][diagonal.second] < 0)
        {
            std::shared_ptr<Move> move = std::make_shared<DirectionalMove>(direction, position);
            moves.push_back(move);
        }

        if (captures_enpassant(boardState, diagonal))
        {
            std::shared_ptr<Move> move = std::make_shared<EnPassantCapture>(direction, position);
            moves.push_back(move);
        }
    }
}

void add_opening_move(Moves &moves, BoardState boardState, Position position)
{
    if (position.second == (boardState.color == COLOR_WHITE ? 1 : 6))
    {
        std::shared_ptr<Move> move = std::make_shared<PawnOpeningMove>(Direction{0, boardState.color * 2}, position);
        moves.push_back(move);
    }
}

void add_standard_move(Moves &moves, BoardState boardState, Position position)
{
    std::shared_ptr<Move> move = std::make_shared<PawnMove>(Direction{0, boardState.color}, position);
    moves.push_back(move);
}

Moves Pawn::get_moves(BoardState boardState, Position position)
{
    Moves moves;
    add_standard_move(moves, boardState, position);
    add_opening_move(moves, boardState, position);
    add_diagonal_captures(moves, boardState, position);
    return moves;
}

int Pawn::get_steps() { return 1; }
int Pawn::get_id() { return color * 6; }