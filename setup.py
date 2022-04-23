from setuptools import setup
from glob import glob 
from pybind11.setup_helpers import Pybind11Extension, build_ext

setup(
    ext_modules=[
        Pybind11Extension("chesscpp", sorted(glob("./chesscpp/src/*.cpp")))
    ],
    name="chess-ai",
    cmdclass={"build_ext": build_ext},
    packages=['chesspy']
)