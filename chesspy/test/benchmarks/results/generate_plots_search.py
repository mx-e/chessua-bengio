import plotly.figure_factory as ff
import plotly.express as px
import pandas as pd, numpy as np
import plotly.graph_objects as go

from matplotlib import pyplot as plt

MAX_DEPTH = 7

data = pd.read_csv('./data/results_depth_7.csv')





print(data)
print(data["depth"] == MAX_DEPTH)
data = data[data["depth"] == MAX_DEPTH]
data['time_passed'] = pd.to_numeric(data["time_passed"])
print(data)
r = data[["run_name", "time_passed"]].sort_values(by="time_passed").groupby("run_name")['time_passed'].apply(list)
r2 = data[["run_name", "time_passed", "nodes_visited"]].sort_values(by="time_passed").groupby("run_name")['nodes_visited'].apply(list)

fig = go.Figure()

runs = ["bare", "pvs", "pvs+qs", "aspiration", "hash", "hash+aspiration"]

for run in runs:
    fig.add_trace(go.Violin(x=data['run_name'][data['run_name'] == run],
                            y=data['time_passed'][data['run_name'] == run],
                            name=run,
                            box_visible=True,
                            meanline_visible=True,
                            points="all"))

fig.show()

fig = go.Figure()

for run in runs:
    fig.add_trace(go.Violin(x=data['run_name'][data['run_name'] == run],
                            y=data['nodes_visited'][data['run_name'] == run],
                            name=run,
                            box_visible=True,
                            meanline_visible=True,
                            points="all"))

fig.show()

# # Create distplot with custom bin_size
# fig = ff.create_distplot(list(r), list(r.index), bin_size=0.2)
# fig.show()