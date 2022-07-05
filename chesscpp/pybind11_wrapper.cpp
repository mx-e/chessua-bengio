#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "include/main.hpp"

namespace py = pybind11;

PYBIND11_MODULE(chesscpp, m)
{
    m.doc() = "Chess-AI C++ Backend";
    m.def("generate_moves", &generate_moves, "Generates all possible moves given an initial state.");
    m.def("runtime_benchmark", &runtime_benchmark, "Runs runtime benchmark for bitboard move generation n times.");
    m.def("bestmove", &bestmove, "");
    m.def("bestmove_benchmark", &bestmove_benchmark, "");
    m.def("evaluate", &evaluate, "");
    // m.def("evaluate_benchmark", &evaluate_benchmark, "");
}