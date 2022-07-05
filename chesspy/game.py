from dataclasses import dataclass
import numpy as np
from chesspy.utils import (
    board_row_to_repr_str,
    board_row_to_row_str,
    can_castle_arr_to_string,
    export_en_passant_tile,
    row_str_to_board_row,
    get_char_to_int_to_move,
    can_castle_string_to_arr,
    extract_en_passant_tile,
    get_int_to_char_to_move,
)


@dataclass
class GameState:
    board_state: np.array
    to_move: bool
    can_castle: np.array
    en_passant_tile: list
    n_reversible_halfmoves: int
    n_moves: int

    def __repr__(self):
        repr = ""
        board = np.flip(self.board_state.T)
        for row in board:
            repr += board_row_to_repr_str(row) + "\n"
        return repr


def import_fen(fen_str: str):
    assert len(fen_str.split(" ")) == 6
    (
        board_state,
        to_move,
        can_castle,
        en_passant_tile,
        n_reversible_halfmoves,
        n_moves,
    ) = fen_str.split()
    assert n_reversible_halfmoves.isnumeric() and n_moves.isnumeric()
    assert len(board_state.split("/")) == 8

    int_board_state = np.array(
        [row_str_to_board_row(row_str) for row_str in board_state.split("/")], dtype=int
    )
    int_to_move = get_char_to_int_to_move(to_move)
    arr_can_castle = can_castle_string_to_arr(can_castle)
    coord_en_passant = extract_en_passant_tile(en_passant_tile)
    return GameState(
        np.flip(int_board_state, axis=0).T,
        int_to_move,
        arr_can_castle,
        coord_en_passant,
        int(n_reversible_halfmoves),
        int(n_moves),
    )


def export_fen(game_state: GameState):
    fen_str = ""
    board = np.flip(game_state.board_state.T, axis=0)
    for i, row in enumerate(board):
        fen_str += board_row_to_row_str(row)
        if i != 7:
            fen_str += "/"
        else:
            fen_str += " "
    fen_str += get_int_to_char_to_move(game_state.to_move) + " "
    fen_str += can_castle_arr_to_string(game_state.can_castle) + " "
    fen_str += export_en_passant_tile(game_state.en_passant_tile) + " "
    fen_str += str(game_state.n_reversible_halfmoves) + " " + str(game_state.n_moves)
    return fen_str
