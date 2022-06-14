from tkinter import N
import unittest, logging, sys, pathlib, json, numpy as np, time
from chesscpp import bestmove
from chesspy.game import import_fen, export_fen


class TestBenchmarks(unittest.TestCase):
    def test_benchmark_bestmove(self):
        log = logging.getLogger("benchmarks.timeManagement")
        state = "5k1r/8/4B3/8/8/8/P7/KQ1N4 w - - 3 40"
        board = import_fen(state)
        enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
        time_left = 180
        max_depth = 10

        start = time.time()
        move = bestmove(time_left, max_depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
        end = time.time()

        print(f"took {end - start} seconds, best move {move}")

        state = "1r6/8/2pbk1p1/p4p2/2KPnP2/4P3/PB4PP/5R2 w - - 0 1"
        board = import_fen(state)
        enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
        time_left = 180
        max_depth = 10

        start = time.time()
        move = bestmove(time_left, max_depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
        end = time.time()

        print(f"took {end - start} seconds, best move {move}")

        state = "rnbqkbnr/1pppppp1/p6p/8/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3"
        board = import_fen(state)
        enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
        time_left = 180
        max_depth = 10

        start = time.time()
        move = bestmove(time_left, max_depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
        end = time.time()



if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.timeManagement").setLevel(logging.DEBUG)
    unittest.main()
