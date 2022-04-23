#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "main.hpp"

PYBIND11_MODULE(chesscpp, m) {
    m.doc() = "Chess-AI C++ Backend";
    m.def("BIG_LOOP", &big_loop, "Runs a big loop (for benchmarking/stat-checking)");
    m.def("get_board", &get_board, "lala");
    m.def("add_1_to_board", &add_1_to_board, "lala");
}