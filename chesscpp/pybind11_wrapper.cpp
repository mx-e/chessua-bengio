#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "src/main.hpp"

namespace py = pybind11;

PYBIND11_MODULE(chesscpp, m) {
    m.doc() = "Chess-AI C++ Backend";
    m.def("get_board", &get_board, "lala");
    m.def("get_possible_boards", &get_possible_boards, "test");
    m.def("generate_moves", &generate_moves, "Generates all possible moves given an initial state.");
    m.def("runtime_benchmark", &runtime_benchmark, "");
}