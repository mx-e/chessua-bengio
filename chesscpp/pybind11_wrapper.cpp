#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "include/main.hpp"

namespace py = pybind11;

PYBIND11_MODULE(chesscpp, m)
{
    m.doc() = "Chess-AI C++ Backend";
    m.def("generate_moves", &generate_moves, "Generates all possible moves given an initial state.");
    m.def("runtime_benchmark", &runtime_benchmark, "Runs runtime benchmark for bitboard move generation n times.");
    m.def("runtime_benchmark_bestmove", &runtime_benchmark_bestmove, "");
    m.def("runtime_benchmark_evaluate", &runtime_benchmark_evaluate, "");
}