
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





# const int white_kingside = 0;
# const int white_queenside = 1;
# const int black_kingside = 2;
# const int black_queenside = 3;