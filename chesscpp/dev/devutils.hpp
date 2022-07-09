#ifndef DEVUTILS_H
#define DEVUTILS_H

void print_bitboard(uint64_t bb);
void print_all_knight_move_boards();
void bitboard_table_from_logic(bool(logic)(int col, int row, int c, int r, float color), float color);

#endif