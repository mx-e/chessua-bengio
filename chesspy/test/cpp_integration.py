import unittest 
from chesscpp import get_board, get_possible_boards

class CPPIntegration(unittest.TestCase):
    def test_possible_boards(self):
        board = get_board()
        boards = get_possible_boards(board, 1)
        self.assertEqual(len(boards), 0)
