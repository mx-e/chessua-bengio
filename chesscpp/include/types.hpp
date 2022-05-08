#include <array>
#include <vector>
#include <tuple>
#include <optional>
#include <string>

typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;
typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;

Position operator+(const Position &x, const Position &y);

const int COLOR_WHITE = 1;
const int COLOR_BLACK = -1;

/**
 * @brief Indicates the queen- and king-side CastlingRights for a contextually available color.
 * 
 */
struct ColorCastlingRights
{
    bool queenSide;
    bool kingSide;
};

/**
 * @brief Indicates the CastlingRights of both the white and black player.
 * 
 */
struct CastlingRights
{
    ColorCastlingRights white; 
    ColorCastlingRights black;
};

typedef std::vector<Position> EnPassants;

/**
 * @brief Specifies a rectangular Window to the Board that is to be considered for performing checks.
 * If not attached to the BoardState the whole Board is considered. The Board perspective is sideways,
 * that is A-H indicate the rows and 1-8 the columns.
 * 
 */
struct Window
{
    /**
     * @brief Starting row-index of the Window.
     * 
     */
    int x;

    /**
     * @brief Starting column-index of the Window.
     * 
     */
    int y;

    /**
     * @brief Ending row-index of the Window.
     * 
     */
    int x_;

    /**
     * @brief Ending column-index of the Window.
     * 
     */
    int y_;
};

/**
 * @brief BoardState indicating all stateful information of the current game state.
 * All parameters are available publicily as to avoid costly getter and setter calls, which is 
 * necessary since the BoardState is constantly read and manipulated throughout the move generation.
 * 
 */
class BoardState
{
public:

    /**
     * @brief Construct a new BoardState from standard parameters such as the sideways Board (see Window), the 
     * color of the current player (-1 for black and 1 for white) and the number of half and full moves.
     * 
     * @param board 
     * @param color 
     * @param halfMove 
     * @param fullMove 
     */
    BoardState(Board board, int color, int halfMove = 0, int fullMove = 0);

    /**
     * @brief Sideways Board containing the Position of each Piece with its Piece#get_id. 
     * 
     */
    Board board;

    /**
     * @brief Color of the current player.
     * 
     */
    int color;

    /**
     * @brief Number of half moves, that is the number of moves since the last capture.
     * 
     */
    int halfMove;

    /**
     * @brief Number of full moves (duration of the game in moves).
     * 
     */
    int fullMove;

    /**
     * @brief Current CastlingRights for both black and white player.
     * 
     */
    CastlingRights castlingRights;

    /**
     * @brief En-passant targets in the current BoardState.
     * 
     */
    EnPassants enpassant;

    /**
     * @brief Rectangular Window to consider during move generation.
     * 
     */
    Window window;

    /**
     * @brief UCI string that corresponds to the last performed Move.
     * 
     */
    std::string uci;

    /**
     * @brief A raw pointer to the previous BoardState for move selection.
     * 
     */
    BoardState* previousState;
};

typedef std::vector<BoardState> BoardStates;

BoardStates get_possible_boards(BoardState boardState);
