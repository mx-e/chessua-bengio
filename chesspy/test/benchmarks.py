import json
from tkinter import N
import unittest
import logging
import sys
import pathlib
import json
import numpy as np
import time
from chesscpp import runtime_benchmark, generate_moves
from chesspy.game import import_fen


def report_speed_differences(topic: "str", unit: "str", current: "float", latest: "float") -> "float":
    log = logging.getLogger("benchmarks.speedDifferences")
    log.setLevel(level=logging.DEBUG)

    if current > latest:
        log.debug(
            f"\033[92mTime Improvement\033[0m of {abs(current - latest):.6f} [{unit}] on {topic}")
        return latest
    elif current == latest:
        log.debug(f"\033[0mSame Time\033[0m on {topic}")
    else:
        log.debug(
            f"\033[91mWorse Time Than ATB\033[0m of {abs(current - latest):.6f} [{unit}] on {topic}")

    return current


class TestBenchmarks(unittest.TestCase):
    def test_benchmark_move_generator(self):
        log = logging.getLogger("benchmarks.moveGenerator")
        log.debug(
            f"\n\nTESTING MAILBOX:"
        )
        test_dir = pathlib.Path(__file__).parent.resolve()
        with open(f"{test_dir}/data/benchmarks.json") as f:
            move_gen_bench = json.loads(f.read())["move_generator"]
        for key, bench in move_gen_bench.items():
            states, n_runs = bench["states"], bench["n_runs"]
            log.debug(
                f"Running move generation benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
            )
            times = []
            n_moves = []
            for state in states:
                board = import_fen(state)
                start = time.time()

                en_passant = board.en_passant_tile if len(board.en_passant_tile) > 0 else []
                runtime_benchmark(board.board_state, board.to_move,en_passant, *
                                  board.can_castle, board.n_reversible_halfmoves, board.n_moves, n_runs)
                end = time.time()
                times.append(end - start)

                #uci_moves = generate_moves(board.board_state, board.to_move, enpassant,
                                          # *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
                n_moves.append(1)

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
            moves_per_s = n_runs / (np.array(times) / np.array(n_moves)).mean()
            log.debug(
                f"{moves_per_s} moves per second."
            )


if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.moveGenerator").setLevel(logging.DEBUG)
    unittest.main()
