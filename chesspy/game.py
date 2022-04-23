from dataclasses import dataclass
import numpy as np
from chesspy.utils import board_row_to_repr_str, row_str_to_board_row, get_char_to_int_to_move, can_castle_string_to_arr, extract_en_passant_tile

@dataclass
class GameState:
    board_state: np.array
    to_move: bool
    can_castle: np.array
    en_passant_tile: np.array
    n_reversible_halfmoves: int
    n_moves: int

    def __repr__(self):
        repr = ''
        for row in self.board_state:
            repr += board_row_to_repr_str(row) + '\n'
        return repr

def import_fen(fen_str:str):
    assert(len(fen_str.split(' ')) == 6)
    board_state, to_move, can_castle, en_passant_tile, n_reversible_halfmoves, n_moves = fen_str.split()
    assert(n_reversible_halfmoves.isnumeric() and n_moves.isnumeric())

    int_board_state = np.array([row_str_to_board_row(row_str) for row_str in board_state.split('/')], dtype=int)
    int_to_move = get_char_to_int_to_move(to_move)
    arr_can_castle = can_castle_string_to_arr(can_castle)
    coord_en_passant = extract_en_passant_tile(en_passant_tile)
    return GameState(int_board_state, int_to_move, arr_can_castle, coord_en_passant, int(n_reversible_halfmoves), int(n_moves))
