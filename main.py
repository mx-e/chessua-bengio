from chesspy.game import import_fen
from chesscpp import generate_moves, bb_generate_moves


fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
board = import_fen(fen_string)
print(board.board_state)

enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
uci_moves = generate_moves(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
uci_moves_list = sorted(uci_moves)
print(uci_moves_list)

print(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)

uci_moves = bb_generate_moves(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
uci_moves_list = sorted(uci_moves)
print(uci_moves_list)