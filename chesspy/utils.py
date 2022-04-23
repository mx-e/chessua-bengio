import numpy as np

reverse_map = lambda map_:{ v: k for k, v in map_.items() }

board_state_char_to_int_map = {'p':-1, 'P':1, 'r':-2, 'R':2, 'n':-3, 'N':3, 'b':-4, 'B':4, 'q':-5, 'Q':5, 'k':-6, 'K':6}
board_state_int_to_char_map = reverse_map(board_state_char_to_int_map)

get_char_to_int_tile_state = lambda char: board_state_char_to_int_map.get(char, 0)
get_int_to_char_tile_state = lambda int: board_state_int_to_char_map.get(int, ' ')

to_move_char_to_int_map = {'w': 0, 'b': 1}
to_move_int_to_char_map = reverse_map(to_move_char_to_int_map)

get_char_to_int_to_move = lambda char: to_move_char_to_int_map.get(char, -1)
get_int_to_char_to_move = lambda int: to_move_int_to_char_map.get(int, '-')

can_castle_char_to_idx_map = {'K':0, 'Q':1, 'k':2, 'q':3}
can_castle_idx_to_char_map = reverse_map(can_castle_char_to_idx_map)

get_char_to_idx_can_castle = lambda char: can_castle_char_to_idx_map.get(char, -1)
get_idx_to_char_can_castle = lambda int: can_castle_idx_to_char_map.get(int, '-')

abc_to_num = lambda char: ord(char) - 96 if ord(char) > 96 and ord(char) < 123 else -1
num_to_abc = lambda i: chr(i + 96) if i>1 and i<27 else ' '

def board_row_to_repr_str(board_row:np.array):
    row_str = ''
    for tile in board_row:
        char_tile_state= get_int_to_char_tile_state(tile)
        row_str += '◇' if char_tile_state == ' ' else char_tile_state
            
    
    return row_str

def board_row_to_row_str(board_row:np.array):
    row_str = ''
    consec_empty = 0
    for tile in board_row:
        char_tile_state= get_int_to_char_tile_state(tile)
        if(char_tile_state == ' '):
            consec_empty += 1
        else:
            if(consec_empty > 0):
                row_str += consec_empty
                consec_empty = 0
            row_str += char_tile_state
    if(consec_empty > 0):
        row_str += str(consec_empty)
    return row_str
            
            

def row_str_to_board_row(row_str:str):
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

def can_castle_string_to_arr(castle_str:str):
    can_castle = np.array([False, False, False, False])
    for char in castle_str:
        idx_castle = get_char_to_idx_can_castle(char)
        if(idx_castle == -1):
            return can_castle
        can_castle[idx_castle] = True
    return can_castle

def extract_en_passant_tile(ep_str:str):
    if(ep_str == '-'): return np.array([-1, -1])
    assert(len(ep_str) == 2)
    col, row = ep_str
    return np.array([abc_to_num(col) -1, int(row) - 1])
    