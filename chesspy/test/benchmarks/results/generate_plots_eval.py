import numpy as np
import plotly.graph_objects as go
from plotly.subplots import make_subplots
from graph_utils import COLORS, rgb_to_rgba, read_results_file, save_image, get_run_data

INFILE = "results_eval_benchmark.csv"
OUTFILE = "results_eval.pdf"

get_run_times = lambda data, run: get_run_data(data, run, "time_passed_ns")

if __name__ == "__main__":
    data = read_results_file(INFILE)

    fig1 = make_subplots(
        1,
        2,
        column_widths=[0.37, 0.63],
        specs=[[{"type": "xy"}, {"type": "domain"}]],
        subplot_titles=[
            "Run time distribution of complete evaluation",
            "Avg. run times of evaluation parts (ns)",
        ],
        vertical_spacing=1,
    )

    fig1.add_trace(
        go.Box(
            y=get_run_times(data, "all_enabled"),
            name="Full Evaluation",
            jitter=0.4,
            pointpos=-1.8,
            boxpoints="all",  # represent all points
            boxmean=True,
            marker=dict(size=5.5, line=dict(width=1, color="#222222")),
            marker_color=COLORS[0],
            fillcolor=rgb_to_rgba(COLORS[0], 0.8),
            line=dict(width=2, color="#222222"),
        ),
        row=1,
        col=1,
    )

    fig1.update_yaxes(
        title_text="runtime in ns",
        gridcolor="rgba(200,200,200,0.5)",
        gridwidth=2.5,
        row=1,
        col=1,
    )
    fig1.update_xaxes(
        visible=False,
        row=1,
        col=1,
    )

    partial_evals = {
        "Material": "mat_only",
        "Mobility": "mob_only",
        "Pawn Structure": "ps_only",
        "King Security": "ks_only",
        "Bishop Pair + Rook Open File + PS-Tables": "other_only",
    }
    all_means = [
        np.around(get_run_times(data, run), decimals=0).mean()
        for run in partial_evals.values()
    ]

    fig1.add_trace(
        go.Pie(
            labels=list(partial_evals.keys()),
            values=all_means,
            hole=0.3,
            textinfo="value",
            textfont_size=15,
            marker=dict(colors=COLORS[1:], line=dict(color="#222222", width=2)),
        ),
        row=1,
        col=2,
    )

    fig1.update_layout(
        height=720,
        width=1280,
        paper_bgcolor="rgba(0,0,0,0)",
        plot_bgcolor="rgba(0,0,0,0)",
        margin=dict(pad=20, r=0),
    )

    fig1.show()
    save_image(fig=fig1, outfile=OUTFILE)
