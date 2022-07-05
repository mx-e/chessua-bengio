from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

class BuildExt(build_ext):
    def build_extension(self, ext) -> None:
        ext.extra_compile_args = ['-g', '-O2']
        return super().build_extension(ext)

setup(
    ext_modules=[
        Pybind11Extension("chesscpp", ["./chesscpp/pybind11_wrapper.cpp"])
    ],
    name="chess-ai",
    cmdclass={"build_ext": build_ext},
    packages=['chesspy']
)