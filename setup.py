from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension("binance_tick_parser.ext", ["lib/bind.cpp"], libraries=['boost_iostreams']),
]

setup(
    name="binance_tick_parser",
    version="0.0.1",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
