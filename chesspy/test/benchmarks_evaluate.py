from tkinter import N
import unittest, logging, sys, pathlib, json, numpy as np, time
from chesscpp import evaluate_benchmark
from chesspy.game import import_fen, export_fen


class TestBenchmarks(unittest.TestCase):
    def test_benchmark_evaluate(self):
        log = logging.getLogger("benchmarks.evaluate")
        test_dir = pathlib.Path(__file__).parent.resolve()
        with open(f"{test_dir}/data/benchmarks.json") as f:
            move_gen_bench = json.loads(f.read())["evaluate"]
        for key, bench in move_gen_bench.items():
            states, n_runs = bench["states"], bench["n_runs"]
            log.debug(
                f"Running evaluation benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
            )
            times = []
            n_moves = []
            for state in states:
                board = import_fen(state)
                start = time.time()

                enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
                evaluate_benchmark(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves,  n_runs)
                end = time.time()
                times.append(end - start)

                # uci_moves = generate_moves(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
                # n_moves.append(len(uci_moves))

            mean_time_million_runs = np.array(times).mean() * (1e6 / n_runs)
            mean_time_single_run_ms = np.array(times).mean() / n_runs * 1000
            log.debug(
                f"Speed Statistics:"
            )
            log.debug(
                f"{mean_time_million_runs:.4f}s per million boards."
            )
            log.debug(
                f"{mean_time_single_run_ms:.6f}ms per positions"
            )
            configs_per_s = n_runs / np.array(times).mean()
            log.debug(
                f"{configs_per_s} boards per second."
            )
            # moves_per_s = n_runs / (np.array(times) / np.array(n_moves)).mean()
            # log.debug(
            #     f"{moves_per_s} moves per second."
            # )

if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.evaluate").setLevel(logging.DEBUG)
    unittest.main()
