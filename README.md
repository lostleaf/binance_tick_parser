# Binance tick data parser

A simple C++ based parser for binance's official tick data with Python interface

## Environment

First, make sure you have `numpy`, `pybind11` and `boost` installed. You may make a new environment by

```
conda create -n binance_tick boost-cpp pybind11 numpy
conda activate binance_tick
```

Also make sure you have compilers install. For example, you should install the `build-essential` package under Ubuntu

```
sudo apt install build-essential
```

Then, build the extensions
```
python setup.py build_ext --inplace
```

You should be able to run the test code

``` 
python test_run.py
```
