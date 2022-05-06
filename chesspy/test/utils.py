import unittest, numpy as np
from chesspy.utils import (
    get_char_to_int_tile_state,
    get_int_to_char_tile_state,
    abc_to_num,
    num_to_abc,
    board_row_to_repr_str,
    board_row_to_row_str,
    row_str_to_board_row,
    can_castle_string_to_arr,
    extract_en_passant_tile,
)


class TestUtils(unittest.TestCase):
    def test_get_char_to_int_tile_state(self):
        pairs = [["P", 6], [22, 0], ["g", 0], ["k", -1]]
        for input, output in pairs:
            self.assertEqual(get_char_to_int_tile_state(input), output)

    def test_get_int_to_char_tile_state(self):
        pairs = [[5, "R"], [1, "K"], [22, " "], ["fhh", " "]]
        for input, output in pairs:
            self.assertEqual(get_int_to_char_tile_state(input), output)

    def test_abc_to_num(self):
        pairs = [["a", 1], ["z", 26], ["Z", 26], ["9", -1], [",", -1]]
        for input, output in pairs:
            self.assertEqual(abc_to_num(input), output)

    def test_num_to_abc(self):
        pairs = [[1, "a"], [14, "n"], ["3", "c"]]
        for input, output in pairs:
            self.assertEqual(num_to_abc(input), output)

    def test_board_row_to_x_and_back(self):
        row1 = np.array([-6, -6, -2, 0, 0, 1, 0, 5])
        row2 = np.array([3, -3, 6, 6, 0, 4, 0, 0])
        row3 = np.array([6, -6, -5, -3, 2, -1, 0, 0])

        self.assertEqual(board_row_to_repr_str(row1), "ppq◇◇K◇R")
        self.assertEqual(board_row_to_repr_str(row2), "NnKK◇B◇◇")
        self.assertEqual(board_row_to_repr_str(row3), "PprbQk◇◇")

        row_string1 = board_row_to_row_str(row1)
        row_string2 = board_row_to_row_str(row2)
        row_string3 = board_row_to_row_str(row3)

        self.assertEqual(row_string1, "ppq2K1R")
        self.assertEqual(row_string2, "NnKK1B2")
        self.assertEqual(row_string3, "PprbQk2")

        self.assertListEqual(list(row1), list(row_str_to_board_row(row_string1)))
        self.assertListEqual(list(row2), list(row_str_to_board_row(row_string2)))
        self.assertListEqual(list(row3), list(row_str_to_board_row(row_string3)))

    def test_can_castle_string_to_arr(self):
        pairs = [
            ["-", [False, False, False, False]],
            ["Kq", [True, False, False, True]],
            ["qk", [False, False, True, True]],
        ]
        for input, output in pairs:
            self.assertListEqual(list(can_castle_string_to_arr(input)), output)

    def test_extract_en_passant_tile(self):
        pairs = [["-", [-1, -1]], ["e5", [4, 4]], ["c8", [2, 7]]]
        for input, output in pairs:
            self.assertListEqual(list(extract_en_passant_tile(input)), output)
