#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "main.hpp"

namespace py = pybind11;

PYBIND11_MODULE(chesscpp, m) {
    m.doc() = "Chess-AI C++ Backend";
    m.def("BIG_LOOP", &big_loop, "Runs a big loop (for benchmarking/stat-checking)");
    m.def("get_board", &get_board, "lala");
    m.def("add_1_to_board", &add_1_to_board, "lala");
    m.def("get_possible_boards", &get_possible_boards, "test");

    /*py::class_<Piece>(m, "Piece")
        .def(py::init<int>())
        .def("get_directions", &Piece::get_directions)
        .def("get_steps", &Piece::get_steps)
        .def("get_id", &Piece::get_id);*/
}