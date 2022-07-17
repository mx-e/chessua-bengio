import pathlib, time
import plotly.express as px
import pandas as pd
import numpy as np

COLORS = px.colors.qualitative.Vivid


def hex_to_rgb(hex: str):
    return rgb_to_rgba(f"rgb{tuple(int(hex[i:i+2], 16) for i in (0, 2, 4))}")


def rgb_to_rgba(rgb: str, a=1.0):
    rgba = rgb[:3] + "a" + rgb[3:-1] + f",{a:.2f}" + rgb[-1]
    return rgba


def read_results_file(file_name: str):
    results_dir = pathlib.Path(__file__).parent.resolve()
    return pd.read_csv(f"{results_dir}/{file_name}")


def save_image(fig, outfile: str):
    results_dir = pathlib.Path(__file__).parent.resolve()
    fig.write_image(f"{results_dir}/figures/{outfile}")
    # workaround for mathjax bug
    time.sleep(1)
    fig.write_image(f"{results_dir}/figures/{outfile}")


def get_run_data(data, run_name: str, col_name: str):
    selected_data = data[data["run_name"] == run_name][col_name].to_list()
    return np.array(selected_data)
