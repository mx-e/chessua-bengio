from tkinter import N
import unittest, logging, sys, pathlib, json, numpy as np, time
from chesscpp import runtime_benchmark
from chesspy.game import import_fen, export_fen


class TestBenchmarks(unittest.TestCase):
    def test_benchmark_move_generator(self):
        log = logging.getLogger("benchmarks.moveGenerator")
        test_dir = pathlib.Path(__file__).parent.resolve()
        with open(f"{test_dir}/data/benchmarks.json") as f:
            move_gen_bench = json.loads(f.read())["move_generator"]
        for key, bench in move_gen_bench.items():
            states, n_runs = bench["states"], bench["n_runs"]
            log.debug(
                f"Running move generation benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
            )
            times = []
            for state in states:
                board = import_fen(state)
                start = time.time()

                enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
                runtime_benchmark(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves, n_runs)
            
                for _ in range(n_runs):
                    export_fen(board)
                ##
                end = time.time()
                times.append(end - start)

            mean_time_million_runs = np.array(times).mean() * (1e6 / n_runs)
            log.debug(
                f"Average speed is {mean_time_million_runs:.4f}s/million boards."
            )
        

if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.moveGenerator").setLevel(logging.DEBUG)
    unittest.main()
