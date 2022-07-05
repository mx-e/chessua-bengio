import plotly.figure_factory as ff
import pandas as pd

data = pd.read_csv('benchmarks.csv')

r = data[["run_name", "time_passed"]].groupby("run_name")['time_passed'].apply(list)

# Create distplot with custom bin_size
fig = ff.create_distplot(list(r), list(r.index))
fig.show()