from http import server
import json
import websocket
import asyncio
import random
import rel


from chesscpp import bestmove

from chesspy.game import import_fen

playerId = 467309
# playerId = 467309
gameId = 68852
username = "chessua_bengio"


server_host = 'ws://koth.df1ash.de:8026'

def on_message(ws, message):
    print(message)
    data = json.loads(message)

    if(data["type"] == 0):
    #   create_game(ws)
      get_games(ws)
    #   join(ws)
    if(data["type"] == 1):
        games = data["games"]
        game  = next((x for x in games if x["ID"] == gameId), None)
        print("###########")
        print(game)
        move(ws, game["fen"])

      
    
    if(data["type"] == 8):
        move(ws, data["fen"])
    
    if(data["type"] == 4):
        move(ws, data["fen"])

    if(data["type"] == 3):
        global gameId
        gameId = data["ID"]
        move(ws, data["fen"])
    


def on_error(ws, error):
    print(error)

def on_close(ws, close_status_code, close_msg):
    print("### closed ###")

def on_open(ws):
    print("Opened connection")
    # login
    ws.send(json.dumps({
        "type": 0,
        "username": username,
        "playerID": playerId
    }))
    # join
    # join(ws)

def join(ws):
    print("Joining game")
    ws.send(json.dumps({
        "type": 3,
        "username": username,
        "playerID": playerId,
        "joinAsPlayer": 1,
        "gameID": gameId
    })) 

def move(ws, fen):
    board = import_fen(fen)
    en_passant = board.en_passant_tile if len(board.en_passant_tile) > 0 else []
    move, _ = bestmove(
        120.0,
        10,
        board.board_state,
        board.to_move,
        en_passant,
        *board.can_castle,
        board.n_reversible_halfmoves,
        board.n_moves
    )
    ws.send(json.dumps({
        "type": 4,
        "playerID": playerId,
        "username": username,
        "gameID": gameId,
        "move": move,
    }))
    print("picked best move: " + move)

def create_game(ws):
    print("Creating game")
    ws.send(json.dumps({
        "type": 2,
        "playerID": playerId,
        "username": username,
    }))

def get_games(ws):
    print("Fetching games")
    ws.send(json.dumps({
        "type": 1,
    }))


if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp(server_host,
                              on_open=on_open,
                              on_message=on_message,
                              on_error=on_error,
                              on_close=on_close)

    ws.run_forever(dispatcher=rel)  # Set dispatcher to automatic reconnection
    rel.signal(2, rel.abort)  # Keyboard Interrupt
    rel.dispatch()