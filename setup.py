from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension("ob_ext", ["lib_cpp/bind.cpp"]),
]

setup(
    name="binance_orderbook",
    version="0.0.1",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
