import unittest
from chesspy.utils import get_char_to_int_tile_state

class TestUtils(unittest.TestCase):
    def test_get_char_to_int_tile_state(self):
        self.assertEqual(get_char_to_int_tile_state("K"),6)
        self.assertEqual(get_char_to_int_tile_state(22), 0)
        self.assertEqual(get_char_to_int_tile_state("g"), 0)
        self.assertEqual(get_char_to_int_tile_state('p'), -1)
        