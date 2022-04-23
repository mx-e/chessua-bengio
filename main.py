import chesspy.utils

def row_str_to_board_row(row_str):
    None

def import_fen(fen_str):
    assert(len(fen_str.split(' ')) == 6)
    board_state, to_move, can_castle, en_passant_tile, n_reversible_halfmoves, n_moves = fen_str.split()
    #    board_row_str = board_state.split('/')


    print(board_state, n_moves)

    return 'TEST'

fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

print(import_fen(fen_string))



