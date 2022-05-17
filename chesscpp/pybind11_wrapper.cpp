#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "include/main.hpp"

namespace py = pybind11;

PYBIND11_MODULE(chesscpp, m)
{
    m.doc() = "Chess-AI C++ Backend";
    m.def("generate_moves", &generate_moves, "Generates all possible moves given an initial state.");
    m.def("runtime_benchmark", &runtime_benchmark, "");
    m.def("bb_generate_moves", &bb_generate_moves, "Generates all possible moves given a state using bitboards.");
    m.def("bb_runtime_benchmark", &bb_runtime_benchmark, "Runs runtime benchmark for bitboard move generation n times.");
}