import unittest, json, pathlib, logging, sys
from chesscpp import generate_moves
from chesspy.game import import_fen


class CPPIntegration(unittest.TestCase):
    # def test_possible_boards(self):
    #     board = get_board()
    #     boards = get_possible_boards(board, 1)
    #     self.assertEqual(len(boards), 0)

    def test_move_generator(self):
        log = logging.getLogger("tests.testMoveGenerator")
        test_dir = pathlib.Path(__file__).parent.resolve()
        log.debug(test_dir)
        with open(f"{test_dir}/data/test_data_all_fen_matches.json") as f:
            test_cases = json.loads(f.read())
        i, n_test_cases = 0, len(test_cases.items())
        log.debug(f"Found {n_test_cases} test cases.")
        for fen, solution in test_cases.items():
            log.debug(f"Testing Case {i+1}/{n_test_cases}: {fen}")
            board = import_fen(fen)
            # TODO: CALL backend here
            test = generate_moves(board)
            print(test)
            uci_moves_list = sorted([])
            n_moves = len(uci_moves_list)

            true_n_legal_moves = solution["n_legal_moves"]
            true_moves_list = sorted(solution["legal_moves"])

            self.assertEqual(
                true_n_legal_moves, true_n_legal_moves
            )  # TODO: insert n generated moves here
            self.assertListEqual(
                true_moves_list, true_moves_list
            )  # TODO: insert generated moves here
            i += 1


if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    # uncomment for debugging
    # logging.getLogger( "tests.testMoveGenerator" ).setLevel( logging.DEBUG )
    unittest.main()
