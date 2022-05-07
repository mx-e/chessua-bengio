#include "main.hpp"

Board get_board()
{
    Board m = {};
    return m;
}

std::map<int, std::string> LETTERS = {{0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {4, "e"}, {5, "f"}, {6, "g"}, {7, "h"}};

std::string uci_part(Position position)
{
    return LETTERS[position.first] + std::to_string(position.second + 1);
}

void add_boards_along(BoardStates &boardStates, BoardState boardState, Move &move, Piece &piece, Position position)
{
    int steps = piece.get_steps();
    move.step();

    while (move.is_possible(boardState) && steps > 0)
    {
        move.update(boardState.board, boardState, piece);

        steps--;

        BoardState newState = prepare_board_state(boardState);

        newState.uci = uci_part(position) + uci_part(move.get_position());

        move.transfer(newState, boardState);
        boardStates.push_back(newState);

        move.step();
    }
}

void add_boards_for_piece(BoardStates &boardStates, BoardState boardState, Piece &piece, Position position)
{
    for (auto move : piece.get_moves(boardState, position))
    {
        add_boards_along(boardStates, boardState, *move, piece, position);
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

UCIStrings generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{

    CastlingRights castlingRights{
        .white = ColorCastlingRights{queenSideWhite, kingSideWhite},
        .black = ColorCastlingRights{queenSideBlack, kingSideBlack}};

    BoardState boardState{board, color, halfMove, fullMove};
    boardState.enpassant = enpassant;
    boardState.castlingRights = castlingRights;

    try
    {
        BoardStates boardStates = get_possible_boards(boardState);
        UCIStrings strings;
        for (BoardState boardState : boardStates)
        {
            try {
                get_possible_boards(boardState);
                strings.push_back(boardState.uci);
            }
            catch (const BoardInCheckException &e)
            {
                // ignore
            }
            
        }
        return strings;
    }
    catch (const BoardInCheckException &e)
    {
        UCIStrings empty;
        return empty;
    }
}

void runtime_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    for(int _ = 0; _ < runs; _++)
    {
        generate_moves(board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    }
}
