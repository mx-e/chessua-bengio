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
    // how to connect check exception in follow-up moves with the castling move?
    /*
    1. (respect) Look-ahead moves: After computing a state, compute all follow-up moves. If check, discard, else
       add look-ahead property to the board state. 
       => this is recursive (bad). 

    2. (regret) Define under which conditions board state would become invalid in hindsight:
       Most typically this is a follow-up check. But in case of castle, this is also related to other board states.
       Idea: Disconnect this step from move generation. Benefits:
       (a) only compute if follow-up move required
       (b) invariant to order of move processing

       But how? 
       Manually: When post-processing castles, identify the boardstates that are transferring the king (this is only ever one
       other state -> easy to find).

       Connection: Ideally, just bubbling up an exception. But this is too impractical due to recursion issues.
       Better: register everything in a manager class (e.g., GameManager), which tracks states and connections.
       Then we employ hindsight_collapse: Once exception of invalid move comes up, look up boards with implications and remove
       them from the queue. BoardStates are considered valid, iff follow-up move generation throws no errors.

       GameManager: doesn't even need to be a class. Pseudo code:

       play:
            states = [state]
            
            invalid_states = []
            next_states = []
            for state in states if state not in invalid_states:
                try:
                    next_states += (all_possible_states(state), state.prev)
                catch InvalidMoveError:
                    invalid_state += state.prev
            
            states = [...states for states, prev in next_states if not prev in invalid_states ]

        How to set prev? Easy generally, just add old state to prev. But what about castling? We know that the other dependent 
        moves are in the same direction loop, so maybe hijack that?

        Idea: Piece already connects the moves. Then simply move all dependents of piece to prev of follow-up state.

        Other idea: Dependent states are connected and each have an invalid flag. If states is invalid it throws exception, sets itself
        and depedent to invalid. Invalid moves are not processed and filtered out.

    */
    Board board = boardState.board;

    return CastlingRights{
        .white = compute_color_castling_rights(boardState.board, COLOR_WHITE),
        .black = compute_color_castling_rights(boardState.board, COLOR_BLACK)
    };
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

        BoardState newState{.board = boardState.board, .color = -boardState.color, .fullMove = boardState.fullMove + 1};
        move.transfer(boardStates, newState, boardState);
    }
}


void add_boards_for_piece(BoardStates &boardStates, BoardState boardState, Piece &piece, int x, int y)
{
    Position position(x, y);

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

    if (!boardState.window)
    {
        boardState.window = Window{0, 0, 8, 8};
    }

    for (int x = boardState.window->x; x < boardState.window->x_; x++)
    {
        for (int y = boardState.window->y; y < boardState.window->y_; y++)
        {
            if (color * board[x][y] > 0)
            {
                Piece &piece = *pieces[board[x][y]];
                add_boards_for_piece(boardStates, boardState, piece, x, y);
            }
        }
    }
    return boardStates;
}
