import sys, pathlib, json, time, csv
import numpy as np
from chesspy.game import import_fen
from chesscpp import bestmove_benchmark

MAX_DEPTH = 6

if __name__ == "__main__":
    test_dir = pathlib.Path(__file__).parent.resolve()
    with open(f"{test_dir}/data/test_data_all_fen_matches.json") as f:
        position_data = json.loads(f.read())

    RunName = sys.argv[1]
    assert(type(RunName) is str)
    overwrite_csv = False if len(sys.argv) == 2 else bool(sys.argv[2])
    assert(type(overwrite_csv) is bool)

    SearchDepths = np.arange(MAX_DEPTH) + 1
    Positions = position_data.keys()
    
    results = [["run_name", "position", "depth", "time_passed", "best_move", "nodes_visited"]]

    for depth in SearchDepths:
        for position in Positions:
            board = import_fen(position)
            start = time.time()
            en_passant = [board.en_passant_tile] if len(board.en_passant_tile) > 0 else []
            best_move, nodes_visited = bestmove_benchmark(depth, board.board_state, board.to_move, en_passant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
            time_passed = time.time() - start
            result = [RunName, position, depth, time_passed, best_move, nodes_visited]
            results.append(result)

    with open(f"{test_dir}/data/results.csv", "a") as f:
        if(overwrite_csv):
            f.truncate(0)
        writer = csv.writer(f)
        writer.writerows(results)



