from dataclasses import dataclass
import enum
import websocket
import json

class MessageType(enum.Enum):
  LOGIN = 0
  GET_GAMES = 1
  CREATE_GAME = 2
  JOIN_GAME = 3
  MOVE = 4
  DATABASE = 5
  START_TOURNAMENT = 6
  GET_PLAYERS = 7
  GAME_STARTED = 8
  ERROR = -1
  ILLEGAL_MOVE = -2
  NOT_IMPLEMENTED = -3
  INVALID_REQUEST = -4
  UNAUTHORIZED = -5
  FULL = -6
  NOT_FOUND = -7

GAMEID = 117584

@dataclass
class TournamentClient:
    create_game: "bool" = False
    join_game: "bool" = False

    def __post_init__(self):
        websocket.enableTrace(True)
        self.ws = websocket.WebSocketApp("wss://koth.df1ash.de",
                                on_open=self.on_open,
                                on_message=self.on_message,
                                on_error=self.on_error,
                                on_close=self.on_close)
    def listen(self):
        self.ws.run_forever()

    def on_message(self, ws, message):
        message = json.loads(message)

        import pdb; pdb.set_trace()

        if message['type'] == MessageType.LOGIN.value:
            if self.create_game:
                ws.send(json.dumps({
                    "type": MessageType.CREATE_GAME.value,
                    "username": message['playerName'],
                    "playerID": message['playerID']
                }))
            elif self.join_game: 
                ws.send(json.dumps({
                    "type": MessageType.JOIN_GAME.value,
                    "username": message['playerName'],
                    "playerID": message['playerID'],
                    "joinAsPlayer": 1,
                    "gameID": GAMEID
                }))
            else:
                ws.send(json.dumps({
                    "type": MessageType.MOVE.value,
                    "username": "chessua-bengio", 
                    "playerID": "741782",
                    "gameID": GAMEID,
                    "move": "d2d4"
                }))

    def on_error(self, ws, error):
        print(error)

    def on_close(self, ws, close_status_code, close_msg):
        print("### closed ###")

    def on_open(self, ws):
        print("Opened connection")
        ws.send(json.dumps({
            "type": MessageType.LOGIN.value,
            "username": "chessua-bengio", 
            "playerID": "741782"
        }))

if __name__ == "__main__":
    TournamentClient().listen()

    