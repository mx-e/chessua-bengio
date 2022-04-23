from chesspy.utils import *
import numpy as np

def row_str_to_board_row(row_str):
    int_board_row = []
    for char in row_str:
        int_tile_state = get_char_to_int_tile_state(char)
        if(int_tile_state != 0):
            int_board_row.append(int_tile_state)
        else:
            n_empty_fields = int(char)
            for _ in range(n_empty_fields):
                int_board_row.append(0) 
    return int_board_row

def can_castle_string_to_arr(castle_str):
    can_castle = np.array([False, False, False, False])
    for char in castle_str:
        idx_castle = get_char_to_idx_can_castle(char)
        if(idx_castle == -1):
            return can_castle
        can_castle[idx_castle] = True
    return can_castle

def extract_en_passant_tile(ep_str):
    if(ep_str == '-'): return np.array([-1, -1])
    assert(len(ep_str) == 2)
    col, row = ep_str
    return np.array([abc_to_num(col) -1, int(row) - 1])
    

def import_fen(fen_str):
    assert(len(fen_str.split(' ')) == 6)
    board_state, to_move, can_castle, en_passant_tile, n_reversible_halfmoves, n_moves = fen_str.split()
    assert(n_reversible_halfmoves.isnumeric(), n_moves.isnumeric())

    int_board_state = np.array([row_str_to_board_row(row_str) for row_str in board_state.split('/')], dtype=int)
    int_to_move = get_char_to_int_to_move(to_move)
    arr_can_castle = can_castle_string_to_arr(can_castle)
    coord_en_passant = extract_en_passant_tile(en_passant_tile)
    return (int_board_state, int_to_move, arr_can_castle, coord_en_passant, int(n_reversible_halfmoves), int(n_moves))

fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

print(get_char_to_idx_can_castle("K"))
print(import_fen(fen_string))



