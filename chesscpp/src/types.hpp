#include <array>
#include <vector>
#include <tuple>
#include <optional>

typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;
typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;

Position operator+(const Position &x, const Position &y);

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

struct Window
{
    int x;
    int y;
    int x_;
    int y_;
};

struct BoardState
{
    Board board;
    int color;
    CastlingRights castlingRights;
    EnPassants enpassant;
    int halfMove;
    int fullMove;
    std::optional<Window> window;
};
