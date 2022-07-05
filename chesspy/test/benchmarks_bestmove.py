from tkinter import N
import unittest, logging, sys, pathlib, json, numpy as np, time
import uuid
from chesscpp import bestmove_benchmark, bestmove_benchmark_marshal
from chesspy.game import import_fen, export_fen


class TestBenchmarks(unittest.TestCase):
    # def test_benchmark_bestmove(self):
    #     log = logging.getLogger("benchmarks.bestMove")
    #     test_dir = pathlib.Path(__file__).parent.resolve()
    #     with open(f"{test_dir}/data/benchmarks.json") as f:
    #         move_gen_bench = json.loads(f.read())["bestmove"]
    #     for key, bench in move_gen_bench.items():
    #         states, n_runs = bench["states"], bench["n_runs"]
    #         log.debug(
    #             f"Running best move benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
    #         )
    #         times = []
    #         n_moves = []
    #         for state in states:
    #             board = import_fen(state)
    #             start = time.time()

    #             log.debug("Testing position with alpha beta pruning.")

    #             enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []

    #             depth = 7
    #             best_move = bestmove_benchmark(depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
    #             log.debug(f"current depth: {depth}; time: {time.time() - start} seconds. bestmove: {best_move}")
                    
                
    #             end = time.time()
    #             times.append(end - start)

    def test_benchmark_bestmove_all(self):
        log = logging.getLogger("benchmarks.bestMove")
        max_depth = 8
        test_dir = pathlib.Path(__file__).parent.resolve()
        results = {}

        with open(f"{test_dir}/data/benchmarks.json") as f:
            move_gen_bench = json.loads(f.read())["bestmove"]
        for key, bench in move_gen_bench.items():
            states, n_runs = bench["states"], bench["n_runs"]

            log.debug(
                f"Running best move benchmark '{key}', with {len(states)} configurations, {n_runs} runs each..."
            )

            for state in states:
                results[state] = []
                board = import_fen(state)
                enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []

                for depth in range(1, max_depth + 1):
                    start = time.time()
                    best_move = bestmove_benchmark_marshal(depth, board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
                    bestmove_result = json.loads(best_move)
                    length = time.time() - start
                    log.debug(f"current depth: {depth}; time: {length} seconds. bestmove: {bestmove_result['bestmove']}. reached nodes: {bestmove_result['reachedNodes']}")
                    result = {
                        "depth": depth,
                        "length": length,
                        "bestmove": bestmove_result['bestmove'],
                        "reached_nodes": bestmove_result['reachedNodes']
                    }
                    results[state].append(result)
                    depth += 1
        
        with open(f"{test_dir}/data/results.json", "r+") as f:
            all_results = json.loads(f.read())
            all_results[str(uuid.uuid4())] = results
            f.seek(0)
            f.write(json.dumps(all_results, indent=2))


if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    logging.getLogger("benchmarks.bestMove").setLevel(logging.DEBUG)
    unittest.main()
