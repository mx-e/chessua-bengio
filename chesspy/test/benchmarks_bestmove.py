from tkinter import N
import unittest, logging, sys, pathlib, json, numpy as np, time
from chesscpp import bestmove_benchmark
from chesspy.game import import_fen, export_fen


class TestBenchmarks(unittest.TestCase):
    def test_benchmark_bestmove(self):
        log = logging.getLogger("benchmarks.bestMove")
        test_dir = pathlib.Path(__file__).parent.resolve()
        with open(f"{test_dir}/data/benchmarks.json") as f:
            move_gen_bench = json.loads(f.read())["bestmove"]
        for key, bench in move_gen_bench.items():
            states, n_runs = bench["states"], bench["n_runs"]
            log.debug(
                f"Running best move benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
            )
            times = []
            n_moves = []
            for state in states:
                board = import_fen(state)
                start = time.time()
                max_time = 10

                log.debug("Testing position with alpha beta pruning.")

                enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
                depth = 1
                while(time.time() - start <= max_time):
                    best_move = bestmove_benchmark(depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
                    log.debug(f"current depth: {depth} time: {time.time() - start} seconds. bestmove: {best_move}")
                    depth += 1
                    
                
                end = time.time()
                times.append(end - start)


if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.bestMove").setLevel(logging.DEBUG)
    unittest.main()
