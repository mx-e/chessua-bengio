#include "../include/main.hpp"
#include "limits"

int reachedNodes = 0;

std::map<int, std::string> LETTERS = {{0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {4, "e"}, {5, "f"}, {6, "g"}, {7, "h"}};

std::string uci_part(Position position)
{
    return LETTERS[position.first] + std::to_string(position.second + 1);
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
            try
            {
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
    for (int _ = 0; _ < runs; _++)
    {
        generate_moves(board, color, enpassant, kingSideWhite, queenSideWhite, kingSideBlack, queenSideBlack, halfMove, fullMove);
    }
}

int evaluate(Board board)
{
    std::map<int, int> scorePerPiece = {
        {0, 0},
        {King::ID, 0},
        {King::ID * -1, 0},
        {Queen::ID, 9},
        {Queen::ID * -1, -9},
        {Rook::ID, 5},
        {Rook::ID * -1, -5},
        {Bishop::ID, 3},
        {Bishop::ID * -1, 3},
        {Knight::ID, 3},
        {Knight::ID * -1, -3},
        {Pawn::ID, 1},
        {Pawn::ID * -1, -1}};

    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int id = board[i].at(j);
            score += scorePerPiece.at(id);
            if ((id == King::ID || -id == King::ID) && j >= 2 && j <= 5 && i >= 2 && i <= 5)
                score += id * 10;
        }
    }
    return score;
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a <= b)
        return a;
    return b;
}

bool is_won(BoardState boardState)
{
    try
    {
        BoardStates boardStates = get_possible_boards(boardState);
        int chessBoardsCount = 0;
        for (BoardState newBoardState : boardStates)
        {
            try
            {
                get_possible_boards(newBoardState);
            }
            catch (const BoardInCheckException &e)
            {
                chessBoardsCount++;
            }
        }
        if (chessBoardsCount == boardStates.size())
        {
            boardState.color = -boardState.color;
            try
            {
                get_possible_boards(boardState);
            }
            catch (const BoardInCheckException &e)
            {
                return true;
            }
        }
    }
    catch (const BoardInCheckException &e)
    {
        return false;
    }

    if (boardState.board[3][3] * -boardState.color == King::ID || boardState.board[3][4] * -boardState.color == King::ID || boardState.board[4][3] * -boardState.color == King::ID || boardState.board[4][4] * -boardState.color == King::ID)
        return true;

    return false;
}

move_result alphabeta(BoardState boardState, int depth, int alpha, int beta, bool alphaBetaEnabled)
{
    move_result result;

    if (is_won(boardState))
    {

        if (boardState.color == 1)
            result.score = std::numeric_limits<int>::min();
        else
            result.score = std::numeric_limits<int>::max();

        result.move = boardState.uci;

        return result;
    }

    if (depth == 0)
    {
        result.score = evaluate(boardState.board);

        result.move = boardState.uci;
        return result;
    }

    try
    {
        BoardStates possibleBoards = get_possible_boards(boardState);

        if (boardState.color == 1)
        {
            int value = std::numeric_limits<int>::min();
            BoardState bestBoard = possibleBoards.at(0);
            for (BoardState newBoardState : possibleBoards)
            {
                reachedNodes++;
                move_result newValue = alphabeta(newBoardState, depth - 1, alpha, beta, alphaBetaEnabled);
                value = max(value, newValue.score);
                if (newValue.score > value)
                    bestBoard = newBoardState;

                if (alphaBetaEnabled && value >= beta)
                    break;
                alpha = max(alpha, value);
            }
            result.score = value;
            result.move = bestBoard.uci;
        }
        else
        {
            int value = std::numeric_limits<int>::max();
            BoardState bestBoard = possibleBoards.at(0);
            for (BoardState newBoardState : possibleBoards)
            {
                reachedNodes++;
                move_result newValue = alphabeta(newBoardState, depth - 1, alpha, beta, alphaBetaEnabled);
                value = min(value, newValue.score);
                if (newValue.score > value)
                    bestBoard = newBoardState;

                if (alphaBetaEnabled && value <= alpha)
                    break;
                beta = min(beta, value);
            }
            result.score = value;
            result.move = bestBoard.uci;
        }
    }
    catch (const BoardInCheckException &e)
    {
        result.score = boardState.color == 1 ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
        result.move = "invalid";

        return result;
    }
    return result;
}

std::string bestmove(BoardState boardState, int depth, bool alphaBetaEnabled)
{
    reachedNodes = 0;
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    move_result result = alphabeta(boardState, depth, alpha, beta, alphaBetaEnabled);

    printf("Reached %d nodes\n", reachedNodes);

    return result.move;
}

std::string bestmove_iterative(BoardState boardState)
{
    int depth = 1;

    std::string result;
    while (depth <= 5)
    {
        result = bestmove(boardState, depth, true);
        depth++;
    }

    return result;
}

std::string runtime_benchmark_bestmove(bool alphaBetaEnabled, Board board, int color, int depth, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    CastlingRights castlingRights{
        .white = ColorCastlingRights{queenSideWhite, kingSideWhite},
        .black = ColorCastlingRights{queenSideBlack, kingSideBlack}};

    BoardState boardState{board, color, halfMove, fullMove};
    boardState.enpassant = enpassant;
    boardState.castlingRights = castlingRights;

    return bestmove(boardState, depth, alphaBetaEnabled);
}

void runtime_benchmark_evaluate(Board board, int runs)
{
    for (int _ = 0; _ < runs; _++)
    {
        evaluate(board);
    }
}