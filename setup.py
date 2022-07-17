from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import os
import json


class BuildExt(build_ext):
    def build_extension(self, ext) -> None:
        version = os.environ.get("CHESSAI_VERSION", None)

        ext.extra_compile_args = ["-g", "-std=c++20", "-O3"]
        if version:
            with open("versions.json", "r") as f:
                versions = json.load(f)
            ext.extra_compile_args += versions[version]["flags"]
        return super().build_extension(ext)


setup(
    ext_modules=[Pybind11Extension("chesscpp", ["./chesscpp/pybind11_wrapper.cpp"])],
    name="chess-ai",
    cmdclass={"build_ext": BuildExt},
    packages=["chesspy"],
)
