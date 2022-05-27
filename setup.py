from setuptools import setup
from glob import glob 
from pybind11.setup_helpers import Pybind11Extension, build_ext

class BuildExt(build_ext):
    def build_extension(self, ext) -> None:
        ext.extra_compile_args = ['-g', '-o3']
        return super().build_extension(ext)

setup(
    ext_modules=[
        Pybind11Extension("chesscpp", sorted(["./chesscpp/pybind11_wrapper.cpp"] + glob("./chesscpp/src/*.cpp")))
    ],
    name="chess-ai",
    cmdclass={"build_ext": build_ext},
    packages=['chesspy']
)