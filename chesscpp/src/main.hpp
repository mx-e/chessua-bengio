#include <array>
#include <vector>
#include <iostream>

typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;
typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;

Board get_board();

Boards get_possible_boards(Board board, int color);

