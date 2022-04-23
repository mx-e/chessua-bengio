from chesspy.game import import_fen

fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
game_state = import_fen(fen_string)
print(game_state)



