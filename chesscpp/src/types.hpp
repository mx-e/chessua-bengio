#include <array>
#include <vector>
#include <tuple>

typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;
typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;
typedef std::tuple<Board, int> BoardState;

const int COLOR_WHITE = 1;
const int COLOR_BLACK = -1;
