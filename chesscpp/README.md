# Chess-AI Backend (C++ Binding)
... adapted from [this example](https://github.com/pybind/python_example).

Install `pybind11` via
```bash
python -m pip install pybind11
```
then run
```bash
python -m pip install .
```
from this folder, which does the entire build (no makefile needed) - although you might need [cmake](https://cmake.org/install/).

The `C++` library can then be invoked within `python` like any other package. For usage and a simple benchmark vs. native `python` see `benchmark.ipynb`.

> **_INFO:_** The build process creates a wheel, which we can also use for distribution/dockerization. Even better: This wheel can be built and released on commit via *GitHub Actions*.

GLHF!