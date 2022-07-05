import json
import pathlib
from prettytable import PrettyTable


def get_rounded_str(x):
    return "%.4f" % x


def get_time(details):
    time = details["length"]
    return get_rounded_str(time)


def get_nodes(details):
    return details["reached_nodes"]


def print_times():
    depth = 8
    test_dir = pathlib.Path(__file__).parent.resolve()

    field_names = ["row"]
    for i in range(1, depth + 1):
        field_names.append(f"depth {i}")

    table = PrettyTable()
    table.field_names = field_names

    with open(f"{test_dir}/data/results.json") as f:
        results = json.loads(f.read())

        for rowname, values in results.items():
            avg = [0] * depth
            for position, details in values.items():
                for index, detail in enumerate(details):
                    avg[index] += detail["length"]
            for i in range(len(avg)):
                avg[i] = "=====" + get_rounded_str(avg[i] / len(values))
            table.add_row([rowname + "  avg"] + avg)
            for position, details in values.items():
                table.add_row([position] + list(map(get_time, details)))

    table.align = "r"
    print(table.get_string(fields=["row", "depth 1", "depth 6", "depth 7", "depth 8"]))


def print_nodes():
    depth = 8
    test_dir = pathlib.Path(__file__).parent.resolve()

    field_names = ["row"]
    for i in range(1, depth + 1):
        field_names.append(f"depth {i}")

    table = PrettyTable()
    table.field_names = field_names

    with open(f"{test_dir}/data/results.json") as f:
        results = json.loads(f.read())

        for rowname, values in results.items():
            avg = [0] * depth
            for position, details in values.items():
                for index, detail in enumerate(details):
                    avg[index] += detail["reached_nodes"]
            for i in range(len(avg)):
                avg[i] = get_rounded_str(avg[i] / len(values))
            table.add_row([rowname + "  avg"] + avg)
            for position, details in values.items():
                table.add_row([position] + list(map(get_nodes, details)))

    table.align = "r"
    print(table.get_string(fields=["row", "depth 1", "depth 6", "depth 7", "depth 8"]))


def main():
    print_times()
    print_nodes()


if __name__ == "__main__":
    main()
