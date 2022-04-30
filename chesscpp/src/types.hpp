#include <array>
#include <vector>
#include <tuple>

typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;
typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;

const int COLOR_WHITE = 1;
const int COLOR_BLACK = -1;

struct ColorCastlingRights
{
    bool queenSide;
    bool kingSide;
};

struct CastlingRights
{
    ColorCastlingRights white; 
    ColorCastlingRights black;
};

typedef std::vector<Position> EnPassants;

struct BoardState
{
    Board board;
    int color;
    CastlingRights castlingRights;
    EnPassants enpassant;
    int halfMove;
    int fullMove;
};
