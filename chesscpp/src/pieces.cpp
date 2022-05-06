#include "pieces.hpp"

Piece::Piece(int color)
{
    this->color = color;
}

int Piece::get_color()
{
    return color;
}

int Piece::get_character()
{
    return '-';
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
int King::get_id() { return color * King::ID; }

Moves Queen::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}}, position);
}

int Queen::get_steps() { return 8; }
int Queen::get_id() { return color * 2; }
int Queen::get_character() { return 'q'; }

Moves Bishop::get_moves(BoardState boardState, Position position)
{
    return from_directions({{-1, 0}, {1, -1}, {-1, 1}, {1, 1}}, position);
}

int Bishop::get_steps() { return 8; }
int Bishop::get_id() { return color * 3; }
int Bishop::get_character() { return 'b'; }

Moves Knight::get_moves(BoardState boardState, Position position)
{
    return from_directions({{2, 1}, {1, 2}, {-2, 1}, {-1, 2}}, position);
}

int Knight::get_steps() { return 1; }
int Knight::get_id() { return color * 4; }
int Knight::get_character() { return 'n'; }

Moves Rook::get_moves(BoardState boardState, Position position)
{
    return from_directions({{0, 1}, {1, 0}, {0, -1}, {-1, 0}}, position);
}

int Rook::get_steps() { return 8; }
int Rook::get_id() { return color * Rook::ID; }
int Rook::get_character() { return 'r'; }

bool captures_enpassant(BoardState boardState, Position diagonal)
{
    if (boardState.enpassant.size() > 0)
    {
        auto position = std::find(boardState.enpassant.begin(), boardState.enpassant.end(), diagonal);
        return position != boardState.enpassant.end();
    }
    return false;
}

void add_diagonal_captures(Moves &moves, BoardState boardState, Position position)
{
    for (Direction direction : Directions{{1, boardState.color}, {-1, boardState.color}})
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
    if (position.second != (boardState.color == COLOR_WHITE ? 6 : 1))
    {
        std::shared_ptr<Move> move = std::make_shared<PawnMove>(Direction{0, boardState.color}, position);
        moves.push_back(move);
    }
}

template <class T>
void add_swap_piece(Moves &moves, BoardState boardState, Position position)
{
    std::shared_ptr<Piece> piece = std::make_shared<T>(boardState.color);
    std::shared_ptr<Move> move = std::make_shared<PawnSwapMove>(Direction{0, boardState.color}, position, piece);
    moves.push_back(move);
}

void add_swap_moves(Moves &moves, BoardState boardState, Position position)
{
    if (position.second == (boardState.color == COLOR_WHITE ? 6 : 1))
    {
        add_swap_piece<Queen>(moves, boardState, position);
        add_swap_piece<Knight>(moves, boardState, position);
        add_swap_piece<Bishop>(moves, boardState, position);
        add_swap_piece<Rook>(moves, boardState, position);
    }
}

Moves Pawn::get_moves(BoardState boardState, Position position)
{
    Moves moves;
    add_standard_move(moves, boardState, position);
    add_opening_move(moves, boardState, position);
    add_diagonal_captures(moves, boardState, position);
    add_swap_moves(moves, boardState, position);
    return moves;
}

int Pawn::get_steps() { return 1; }
int Pawn::get_id() { return color * 6; }

BoardState prepare_board_state(BoardState boardState)
{
    CastlingRights rights{
        .white = ColorCastlingRights{
            .queenSide = boardState.castlingRights.white.queenSide && boardState.board[4][0] == King::ID && boardState.board[0][0] == Rook::ID,
            .kingSide = boardState.castlingRights.white.kingSide && boardState.board[4][0] == King::ID && boardState.board[7][0] == Rook::ID},
        .black = ColorCastlingRights{.queenSide = boardState.castlingRights.black.queenSide && boardState.board[4][0] == -King::ID && boardState.board[0][0] == -Rook::ID, .kingSide = boardState.castlingRights.black.kingSide && boardState.board[4][0] == -King::ID && boardState.board[7][0] == -Rook::ID},
    };
    BoardState newState{boardState.board, -boardState.color, boardState.halfMove, boardState.fullMove + 1};
    newState.castlingRights = rights;

    return newState;
}