import plotly.graph_objects as go
from plotly.subplots import make_subplots
from graph_utils import COLORS, rgb_to_rgba, read_results_file, save_image, get_run_data

INFILE = "results_movegen_benchmark.csv"
OUTFILE = "results_movegen.pdf"

# which color from the swatch is picked
COLOR_VAL = 0

if __name__ == "__main__":
    data = read_results_file(INFILE)

    fig1 = make_subplots(
        1,
        2,
        column_widths=[0.37, 0.63],
        specs=[[{"type": "xy"}, {"type": "xy"}]],
        shared_yaxes=True,
        horizontal_spacing=0,
    )

    fig1.add_trace(
        go.Box(
            y=get_run_data(data, "bitboards", "time_passed_ns"),
            name="Full Move Generation",
            boxpoints=False,
            boxmean=True,
            fillcolor=rgb_to_rgba(COLORS[COLOR_VAL], 0.8),
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

    fig1.add_trace(
        go.Scatter(
            x=get_run_data(data, "bitboards", "n_moves"),
            y=get_run_data(data, "bitboards", "time_passed_ns"),
            mode="markers",
            name="individual board states",
            marker=dict(size=7.5, line=dict(width=1, color="#222222")),
            marker_color=COLORS[COLOR_VAL],
        ),
        row=1,
        col=2,
    )
    fig1.update_yaxes(
        gridcolor="rgba(200,200,200,0.5)",
        gridwidth=2.5,
        row=1,
        col=2,
    )
    fig1.update_xaxes(
        title_text="no. moves generated",
        gridcolor="rgba(200,200,200,0.5)",
        gridwidth=2.5,
        row=1,
        col=2,
    )

    fig1.update_layout(
        title=dict(
            text="Run time of move generation",
            x=0.5,
            y=0.9,
            xanchor="center",
            yanchor="top",
            font=dict(
                size=16,
            ),
        ),
        height=720,
        width=1100,
        paper_bgcolor="rgba(0,0,0,0)",
        plot_bgcolor="rgba(0,0,0,0)",
        margin=dict(pad=20, r=0),
    )

    fig1.show()
    save_image(fig=fig1, outfile=OUTFILE)
