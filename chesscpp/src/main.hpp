
typedef std::array<std::array<int, 8>,8> Board;
typedef std::vector<Board> Boards;

float big_loop(int size);
Board get_board();

Board add_1_to_board(Board board);

Boards get_possible_boards(Board board, int color);

typedef std::pair<int, int> Position;
typedef Position Direction;
typedef std::vector<Direction> Directions;