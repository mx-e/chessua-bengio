#include <pybind11/pybind11.h>
#include "main.hpp"

PYBIND11_MODULE(chessai, m) {
    m.doc() = "Chess-AI C++ Backend";
    m.def("BIG_LOOP", &big_loop, "Runs a big loop (for benchmarking/stat-checking)");
}