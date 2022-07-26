#%%
import random, pathlib, csv
from PIL import Image
from fentoboardimage import fenToImage, loadPiecesFolder

def get_random_positions(random_seed: int, pick_n: int):
    random.seed(random_seed)
    test_dir = pathlib.Path(__file__).parent.parent.parent.resolve()
    with open(f"{test_dir}/data/chessData.csv", "r") as f:
        position_data = []
        csvreader = csv.reader(f, delimiter=",")
        for row in csvreader:
            position_data.append(row)
    positions = [
        position_data[i][0] for i in random.sample(range(len(position_data)), pick_n)
    ]
    return positions

def image_grid(imgs, rows, cols, padding):
    assert len(imgs) == rows*cols

    w, h = imgs[0].size
    grid = Image.new('RGB', size=(cols*w + padding*(cols+1), rows*h + padding*(rows+1)))
    grid_w, grid_h = grid.size
    
    for i, img in enumerate(imgs):
        grid.paste(img, box=(padding + i%cols*(w + padding), padding + i//cols*(h+padding)))
    return grid

#%%
RANDOM_SEED =329
boards = get_random_positions(RANDOM_SEED, 10)
imgs = []
for fen in boards:
    print(str(fen))
    board_image = fenToImage(str(fen),120, lightColor="#E7E6E6", darkColor="#7F8470", pieceSet=loadPiecesFolder('./chess_pieces'))
    imgs.append(board_image)
grid = image_grid(imgs=imgs, rows=2, cols=5, padding=10)

# %%

im1 = grid.save("./figures/example_boards.png")

# %%
