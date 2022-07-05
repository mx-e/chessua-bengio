import plotly.figure_factory as ff
import pandas as pd

MAX_DEPTH = 7

data = pd.read_csv('./data/results.csv')

r = data[data['depth'] == MAX_DEPTH]
r = r[["run_name", "time_passed", "depth"]].groupby("run_name")['time_passed'].apply(list)

# Create distplot with custom bin_size
fig = ff.create_distplot(list(r), list(r.index))
fig.show()