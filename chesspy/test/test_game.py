import unittest, logging, sys, pathlib, json
from chesspy.game import import_fen, export_fen

class TestGame(unittest.TestCase):
    def test_fen_to_game_state_to_fen(self):
        log = logging.getLogger("tests.testFenRoundTrip")
        test_dir = pathlib.Path(__file__).parent.resolve()
        with open(f"{test_dir}/data/test_data_all_fen_matches.json") as f:
            test_cases = json.loads(f.read())
        log.debug(f"Found {len(test_cases.items())} test cases.")
        for fen, _ in test_cases.items():
            log.debug(f"Testing FEN: {fen}")
            board = import_fen(fen)
            log.debug(board)
            rt_fen = export_fen(board)
            self.assertEqual(fen, rt_fen)
            assert rt_fen == fen


if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr)
    logging.getLogger("tests.testFenRoundTrip").setLevel(logging.DEBUG)
    unittest.main()
