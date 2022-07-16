import random, pathlib, csv


def get_random_positions(random_seed: int, pick_n: int):
    random.seed(random_seed)
    test_dir = pathlib.Path(__file__).parent.parent.resolve()
    with open(f"{test_dir}/data/fen_positions.csv", "r") as f:
        position_data = []
        csvreader = csv.reader(f, delimiter=",")
        for row in csvreader:
            position_data.append(row)
    positions = [
        position_data[i][0] for i in random.sample(range(len(position_data)), pick_n)
    ]
    return positions


def write_benchmark_results(outname: str, results: list, overwrite=False):
    results_dir = pathlib.Path(__file__).parent.resolve()
    with open(f"{results_dir}/results/{outname}", "a") as f:
        if overwrite:
            f.truncate(0)
        writer = csv.writer(f)
        writer.writerows(results)
