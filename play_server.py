from http import server
import socketio
import asyncio
import random

from chesscpp import runtime_benchmark_bestmove

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
    if " w " in data:
        current_color = "white"
    else:
        current_color = "black"
    if current_color != player_color and player_color != '':
        return
    print("received board, starting ai")
    enpassant = [tuple(board.en_passant_tile)] if not (board.en_passant_tile[0] == -1 and board.en_passant_tile[1] == -1) else []
    move = runtime_benchmark_bestmove(False, board.board_state, board.to_move, 3, enpassant, *board.can_castle, board.n_reversible_halfmoves, board.n_moves)
    print('board state' + data)
    print('picked best move: '+ move)
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
