#include <vector>
#include <string>
#include <sstream>

const int empty = 0;
const int white_pawn = 1;
const int black_pawn = -1;
const int white_rook = 2;
const int black_rook = -2;
const int white_knight = 3;
const int black_knight = -3;
const int white_bishop = 4;
const int black_bishop = -4;
const int white_queen = 5;
const int black_queen = -5;
const int white_king = 6;
const int black_king = -6;

const int a = 0;
const int b = 1;
const int c = 2;
const int d = 3;
const int e = 4;
const int f = 5;
const int g = 6;
const int h = 7;

const bool white = 0;
const bool black = 1;

const int white_kingside = 0;
const int white_queenside = 1;
const int black_kingside = 2;
const int black_queenside = 3;

std::vector<std::string> split_string(std::string str, std::string delimiter);
int fen_char_to_board_state(char const symbol);
int fen_castling_char_to_idx(char const symbol);

// maps small letters to numbers, a->1, b->2, etc.
int abc_to_123(char const col_char);
int char_to_int(char const ch);
char board_state_to_char(int state);
