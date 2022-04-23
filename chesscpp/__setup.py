from setuptools import setup 
from glob import glob 
from pybind11.setup_helpers import Pybind11Extension, build_ext

setup(
    ext_modules=[
        Pybind11Extension("chessai", sorted(glob("src/*.cpp")))
    ],
    name="chessai",
    cmdclass={"build_ext": build_ext}
)