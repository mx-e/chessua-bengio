from http import server
import socketio
import asyncio
import random

from chesscpp import generate_moves

from chesspy.game import import_fen


server_host = 'http://localhost:8080'
sio = socketio.AsyncClient()

player_color = ''



@sio.on('color_assignment')
async def on_color(data):
    global player_color
    player_color = data
    print('player color is ' + data)



@sio.on('board_state')
async def board_state(data):
    board = import_fen(data)
    enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
    uci_moves = generate_moves(board.board_state, board.to_move, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
    print('board state' + data)
    move = random.choice(uci_moves)
    print('picked random move: '+ move)
    await sio.emit('move', move)


@sio.event
def connect():
    print("Connected")


async def main():
  await sio.connect(server_host)
  await sio.emit("join", {})
  print('joined game')
  await sio.wait()

if __name__ == "__main__":
    asyncio.run(main())
