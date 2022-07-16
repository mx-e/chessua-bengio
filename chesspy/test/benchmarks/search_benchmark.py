import sys, time
import numpy as np
from chesspy.game import import_fen
from chesscpp import bestmove_benchmark
from benchmark_utils import get_random_positions, write_benchmark_results

MAX_DEPTH = 7
N_POSITIONS = 10
RANDOM_SEED = 329
OUTNAME = "results_search_benchmark.csv"


if __name__ == "__main__":
    RunName = sys.argv[1]
    assert type(RunName) is str
    overwrite_csv = False if len(sys.argv) == 2 else bool(sys.argv[2])
    assert type(overwrite_csv) is bool

    SearchDepths = np.arange(MAX_DEPTH) + 1
    Positions = get_random_positions(RANDOM_SEED, N_POSITIONS)

    results = (
        [["run_name", "position", "depth", "time_passed", "best_move", "nodes_visited"]]
        if overwrite_csv
        else []
    )

    for depth in SearchDepths:
        for position in Positions:
            board = import_fen(position)
            start = time.time()
            best_move, nodes_visited = bestmove_benchmark(
                depth,
                board.board_state,
                board.to_move,
                board.en_passant_tile,
                *board.can_castle,
                board.n_reversible_halfmoves,
                board.n_moves,
            )
            time_passed = time.time() - start
            result = [RunName, position, depth, time_passed, best_move, nodes_visited]
            results.append(result)
    write_benchmark_results(outname=OUTNAME, results=results, overwrite=overwrite_csv)
