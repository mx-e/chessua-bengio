#include "main.hpp"

Board get_board()
{
    Board m = {};
    return m;
}



ColorCastlingRights compute_color_castling_rights(Board board, int color)
{
    int col = color == COLOR_WHITE ? 0 : 7;
    bool kingSide = board[4][col] == color * 1 && board[5][col] == 0 && board[6][col] == 0 && board[7][col] == color * 5;
    bool queenSide = board[4][col] == color * 1 && board[3][col] == 0 && board[2][col] == 0 && board[1][col] == 0 && board[0][col] == color * 5;
    return ColorCastlingRights{.kingSide = kingSide, .queenSide = queenSide};
}

CastlingRights compute_castling_rights(BoardState boardState)
{
    Board board = boardState.board;

    return CastlingRights{
        .white = compute_color_castling_rights(boardState.board, COLOR_WHITE),
        .black = compute_color_castling_rights(boardState.board, COLOR_BLACK)};
}

void add_boards_along(BoardStates &boardStates, BoardState boardState, Move &move, Piece &piece)
{
    int steps = piece.get_steps();
    move.step();

    while (move.is_possible(boardState) && steps > 0)
    {
        move.update(boardState.board, boardState, piece);
        move.step();
        steps--;

        BoardState newState = prepare_board_state(boardState);
        move.transfer(newState, boardState);
        boardStates.push_back(newState);
    }
}

void add_boards_for_piece(BoardStates &boardStates, BoardState boardState, Piece &piece, Position position)
{
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

    for (int x = boardState.window.x; x < boardState.window.x_; x++)
    {
        for (int y = boardState.window.y; y < boardState.window.y_; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *pieces[board[x][y]];
                add_boards_for_piece(boardStates, boardState, piece, Position(x, y));
            }
        }
    }
    return boardStates;
}
