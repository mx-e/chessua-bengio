import plotly.graph_objects as go
from graph_utils import COLORS, rgb_to_rgba, read_results_file, save_image, get_run_data

INFILE = "results_search_benchmark.csv"
OUTFILE1 = "results_search_dist.pdf"

SEARCH_DEPTH = 7

if __name__ == "__main__":
    data = read_results_file(INFILE)
    data = data[data["depth"] == SEARCH_DEPTH]

    traces = {
        "Alpha/Beta": "bare",
        "Principle Variation Search": "pvs",
        "PV+Q Search (up to depth 11)": "pvs+qs",
        "Previous + Aspiration Search": "aspiration",
        "Previous + Transposition Table": "hash",
        "Previous + Both": "hash+aspiration",
    }

    fig1 = go.Figure()

    i = 0
    for key, value in traces.items():
        fig1.add_trace(
            go.Box(
                jitter=0.7,
                pointpos=-1.95,
                boxmean=True,
                boxpoints="all",
                y=get_run_data(data, value, "time_passed"),
                name=key,
                marker=dict(size=5.5, line=dict(width=1, color="#222222")),
                marker_color=COLORS[i],
                fillcolor=rgb_to_rgba(COLORS[i], 0.8),
                line=dict(width=2, color="#222222"),
            )
        )
        i += 1
    fig1.update_yaxes(
        title_text="runtime in s",
        range=[-3, 35],
        gridcolor="rgba(200,200,200,0.5)",
        gridwidth=2.5,
    )
    fig1.update_layout(
        title=dict(
            text="Run time of move search variants for depth=7",
            x=0.5,
            y=0.9,
            xanchor="center",
            yanchor="bottom",
            font=dict(
                size=16,
            ),
        ),
        showlegend=False,
        height=1000,
        width=1280,
        paper_bgcolor="rgba(0,0,0,0)",
        plot_bgcolor="rgba(0,0,0,0)",
        margin=dict(pad=20, r=0),
    )
    fig1.show()
    save_image(fig=fig1, outfile=OUTFILE1)
